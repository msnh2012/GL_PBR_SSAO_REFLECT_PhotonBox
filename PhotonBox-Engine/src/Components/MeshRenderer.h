#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

class Texture;
#include "ObjectRenderer.h"
#include "../Resources/Mesh.h"
#include "../Resources/Material.h"

class MeshRenderer : public ObjectRenderer {
public:
	MeshRenderer(bool isOpaque) :ObjectRenderer(isOpaque) {}
	MeshRenderer() : ObjectRenderer(true) {}
	void init() override;
	void render() override;
	void render(Shader* shader) override;
	void render(Shader* shader, LightEmitter* light) override;
	void onDestroy() override;

	MeshRenderer& setMesh(Mesh* mesh) { _mesh = mesh; return *this; }
private: 
	Mesh* _mesh;

	//AABB* _lightProbeVolume;
	GLuint _vao, _vbo, _ebo;
};

#endif // MESH_RENDERER_H