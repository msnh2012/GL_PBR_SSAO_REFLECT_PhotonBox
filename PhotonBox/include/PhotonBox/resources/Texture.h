#ifndef TEXTURE_H
#define TEXTURE_H

#include "PhotonBox/core/Display.h"
#include "PhotonBox/core/ManagedResource.h"

class Texture : public ManagedResource
{
public:
	Texture(bool generateMipMaps, bool hdr = false);
	Texture(std::string fileName, bool generateMipMaps = false, bool hdr = false);
	~Texture();
	void bind();
	void bind(GLuint textureUnit);
	int getWidth() { return _width; }
	int getHeight() { return _height; }
	bool isHDR() { return _isHDR; }
	std::string getName() { return _fileName; }
	GLuint getTextureLocation() { return _texture; }

	static unsigned char* loadIcon(const std::string& fileName, int& width, int& height);
private:
	bool _isHDR;
	GLuint _texture;
	int _width, _height;
	std::string _fileName;
	static unsigned int _nameIndex;

	void initializeTexture(unsigned char* data, bool generateMipMaps, bool hdr);
};

#endif // TEXTURE_H
