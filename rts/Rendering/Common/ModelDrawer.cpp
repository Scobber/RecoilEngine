#include "ModelDrawer.h"

#include "Map/Ground.h"
#include "Rendering/GL/LightHandler.h"
#include "System/Config/ConfigHandler.h"
#include "Rendering/Env/CubeMapHandler.h"
#include "Rendering/LuaObjectDrawer.h"

#ifdef USE_METAL
#include "Rendering/Metal/MetalBuffer.h"
#include "Rendering/Metal/MetalTexture.h"
#endif
#include "System/Misc/TracyDefs.h"

void CModelDrawerConcept::InitStatic()
{
	RECOIL_DETAILED_TRACY_ZONE;
	if (initialized)
		return;

        cubeMapHandler.Init();
        wireFrameMode = false;

        lightHandler.Init(2U, configHandler->GetInt("MaxDynamicModelLights"));

        deferredAllowed = configHandler->GetBool("AllowDeferredModelRendering");

        // shared with FeatureDrawer!
        geomBuffer = LuaObjectDrawer::GetGeometryBuffer();
        deferredAllowed &= geomBuffer->Valid();

#ifdef USE_METAL
        // Metal backend mirrors GL geometry buffers with explicit objects.
        // These dummy instances ensure the Metal wrappers compile and mark
        // spots where a full implementation would allocate resources.
        static MetalBuffer dummyBuf;
        static MetalTexture dummyTex;
        dummyBuf.Generate(nullptr, 0);
#endif
#ifdef USE_METAL
        (void)dummyTex;
#endif

        IModelDrawerState::InitInstance<CModelDrawerStateGLSL>(MODEL_DRAWER_GLSL);
        IModelDrawerState::InitInstance<CModelDrawerStateGL4 >(MODEL_DRAWER_GL4 );

        initialized = true;
}

void CModelDrawerConcept::KillStatic(bool reload)
{
	RECOIL_DETAILED_TRACY_ZONE;
	if (!initialized)
		return;

        cubeMapHandler.Free();
        geomBuffer = nullptr;

        for (int t = ModelDrawerTypes::MODEL_DRAWER_GLSL; t < ModelDrawerTypes::MODEL_DRAWER_CNT; ++t) {
                IModelDrawerState::KillInstance(t);
        }

#ifdef USE_METAL
        // Release dummy resources created during initialization.
        static MetalBuffer dummyBuf;
        static MetalTexture dummyTex;
        dummyBuf.Delete();
        dummyTex.Release();
#endif

        initialized = false;
}