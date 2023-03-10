#include "PhotonBox/component/AmbientLight.h"

#include "PhotonBox/core/system/Lighting.h"
#include "PhotonBox/resource/shader/ForwardAmbientLightShader.h"

#ifdef PB_MEM_DEBUG
#include "PhotonBox/util/MEMDebug.h"
#define new DEBUG_NEW
#endif

void AmbientLight::init()
{
	Lighting::addLight(this);
}

void AmbientLight::destroy()
{
	Lighting::removeLight(this);
}

Shader* AmbientLight::getLightShader()
{
	return ForwardAmbientLightShader::getInstance();
}
