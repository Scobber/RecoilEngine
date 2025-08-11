/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#include "MetalRenderTarget.h"

/**
 * @brief MetalRenderTarget implementation
 *
 * GL's FBO allows explicit binding and status checks.  Metal relies on
 * MTLRenderPassDescriptor and encoders; this wrapper stores the descriptor
 * and keeps a similar API for the engine.
 */
MetalRenderTarget::MetalRenderTarget() = default;
MetalRenderTarget::~MetalRenderTarget() { Release(); }

bool MetalRenderTarget::Init()
{
	Release();
	passDesc = nullptr; // [MTLRenderPassDescriptor renderPassDescriptor] in real impl
	return true;
}

void MetalRenderTarget::Release()
{
	passDesc = nullptr;
	colorTex = nullptr;
	depthTex = nullptr;
}

void MetalRenderTarget::AttachTexture(id<MTLTexture> tex, bool depth)
{
	if (depth)
		depthTex = tex;
	else
		colorTex = tex;
	// real implementation would assign attachment of passDesc
}

#endif // USE_METAL
