#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

class DepthShader;
class FrameBuffer;

#include "PhotonBox/component/LightEmitter.h"
#include "PhotonBox/math/Matrix4f.h"

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT DirectionalLight : public LightEmitter
{
public:
	Vector3f direction;
	Matrix4f lightProjection;

	~DirectionalLight();

	void OnEnable() override;
	void OnDisable() override;

	void init() override;
	void destroy() override;
	Shader* getLightShader() override;
	FrameBuffer* getShadowBuffer();
private:
	FrameBuffer* _shadowBuffer;
};

#endif // DIRECTIONAL_LIGHT_H
