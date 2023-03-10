#ifndef RENDERER_H
#define RENDERER_H

class CubeMap;
class FrameBuffer;
class TransparentShader;
class GShader;
class DeferredShader;
class DirectionalShadowShader;
class SSVOShader;
class VolumetricFogShader;
class Material;
class AABB;
class MeshRenderer;

#include <vector>
#include <map>

#include "PhotonBox/resource/shader/ForwardAmbientLightShader.h"
#include "PhotonBox/resource/shader/ForwardDirectionalLightShader.h"
#include "PhotonBox/resource/shader/ForwardPointLightShader.h"
#include "PhotonBox/resource/shader/ForwardSpotLightShader.h"
#include "PhotonBox/resource/SkyBox.h"
#include "PhotonBox/core/ISystem.h"

enum RenderType
{
	RT_OPAQUE, RT_CUTOUT, RT_TRANSPARENT
};

#include "PhotonBox/PhotonBoxConfig.h"

class PB_PORT Renderer : public ISystem
{
public:
	static const int MAX_DEBUG = 4;
	static const int SHADOW_MAP_RESOLUTION = 4096;

	bool depthPrePass = false;

	static int getDebugMode() { return _debugMode; }
	static void setDebug(int debugMode);
	static void addToRenderQueue(ObjectRenderer *renderer, RenderType type);
	static void removeFromRenderQueue(ObjectRenderer *renderer);
	static void setSkyBox(CubeMap* cubeMap);
	static SkyBox* getSkyBox() { return &_skyBox; }
	static void setClearColor(Vector3f color);
	static Vector3f getClearColor() { return _clearColor; }
	static void addDrawCall();
	static int getDrawCalls() { return _drawCalls; }
	static FrameBuffer* getMainFrameBuffer() { return _mainFrameBuffer; }
	static FrameBuffer* getGBuffer() { return _gBuffer; }
	static FrameBuffer* getDebugBuffer() { return _gizmoBuffer; }

	static void render();
	static void populateShadowBuffer();
	
	static void captureScene(LightMap* lightmap = nullptr);
	static unsigned long int getFrameIndex();
	
	void init(Config::Profile profile) override;
	void start() override;
	void reset() override;
	void destroy() override;

	
	void newFrame();
	void prePass();
	void clearDrawCalls();
	void renderGizmos();
	static bool _shadowsAreDirty;
private:
	static int _drawCalls;
	static unsigned int _frameIndex;
	static SkyBox _skyBox;
	static FrameBuffer* _mainFrameBuffer;
	static FrameBuffer* _gBuffer;
	static FrameBuffer*	_gizmoBuffer;
	static FrameBuffer* _shadowBuffer;
	static int _debugMode;
	static std::vector<ObjectRenderer*> _renderListOpaque;
	static std::vector<ObjectRenderer*> _renderListTransparent;
	static std::vector<ObjectRenderer*> _renderListCustom;
	static Vector3f _clearColor;
	static Texture* _noise;
	static Texture* _vec_noise;

	static std::map<float, MeshRenderer*> _renderQueueTransparent;

	static ForwardAmbientLightShader* _ambientLightShader;
	static ForwardDirectionalLightShader* _directionalLightShader;
	static ForwardPointLightShader* _pointLightShader;
	static ForwardSpotLightShader* _spotLightShader;
	static TransparentShader* _transparentBaseShader;
	static DeferredShader* _deferredShader;
	static DirectionalShadowShader* _directionalShadowShader;
	static SSVOShader* _ssvoShader;
	static VolumetricFogShader* _volumetricFogShader;
	static Material* _deferredMaterial;
	static Material* _ssvoMaterial;
	static Material* _volumetricFogMaterial;

	static void renderDeferred();
	static void renderForward();
	static void renderTransparents();
	static void renderCustoms();
	static void ssShadowPass();
	static void renderFog();

	static void updateTransparentQueue();
};

#endif // RENDERER_H
