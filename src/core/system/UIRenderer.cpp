#include "PhotonBox/core/system/UIRenderer.h"

#include <iostream>

#include "PhotonBox/resource/shader/TextShader.h"
#include "PhotonBox/core/Resources.h"
#include "PhotonBox/core/system/Renderer.h"
#include "PhotonBox/util/Logger.h"
#include "PhotonBox/core/OpenGL.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

TextShader* UIRenderer::shader;
GLuint UIRenderer::_VAO, UIRenderer::_VBO;
std::map<GLchar, Character> UIRenderer::_characters;

void UIRenderer::init(Config::Profile profile)
{
	Logger::infoln("Initializing UIRenderer");

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		Logger::errln("Could not init FreeType Library");

	FT_Face face;
	if (FT_New_Face(ft, (Resources::ENGINE_RESOURCES + "/fonts/OpenSans.ttf").c_str(), 0, &face))
		Logger::errln("Failed to load font");


	FT_Set_Pixel_Sizes(face, 0, 48);


	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; ++c)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Logger::errln("Failed to load Glyph");
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			Vector2f(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
			Vector2f(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		_characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	//glEnableVertexAttribArray(0);
	glVertexAttribPointer(Vertex::AttibLocation::POSITION, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader = TextShader::getInstance();
}

void UIRenderer::renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, Vector3f color)
{
	GLfloat initialX = x;
	// Activate corresponding render state	
	shader->bind();
	shader->update();
	shader->setUniform("textColor", color);
	shader->updateTextures();

	//glUniform3f(glGetUniformLocation(shader.Program, "textColor"), color.x(), color.y(), color.z());
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c)
	{
		Character ch = _characters[*c];

		if (*c == '\n')
		{
			y -= 48 * scale;
			x = initialX;
			continue;
		}

		GLfloat xpos = x + ch.Bearing.x() * scale;
		GLfloat ypos = y - (ch.Size.y() - ch.Bearing.y()) * scale;

		GLfloat w = ch.Size.x() * scale;
		GLfloat h = ch.Size.y() * scale;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shader->enableAttributes();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Renderer::addDrawCall();
		shader->disableAttributes();
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
