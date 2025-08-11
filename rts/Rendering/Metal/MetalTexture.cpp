/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#include "MetalTexture.h"

/**
 * @brief MetalTexture implementation
 *
 * Differs from GL textures in that Metal uses descriptors and has no
 * global binding points.  Functions therefore operate directly on the
 * underlying id<MTLTexture> without glBind style calls.
 */
MetalTexture::MetalTexture() = default;
MetalTexture::~MetalTexture() { Release(); }

void MetalTexture::Allocate(id<MTLDevice> device, size_t width, size_t height, MTLPixelFormat format)
{
	Release();
	xsize = width;
	ysize = height;
	texture = nullptr; // device->newTextureWithDescriptor would go here
}

void MetalTexture::Release()
{
	if (texture == nullptr)
		return;
	texture = nullptr; // release in real implementation
}

void MetalTexture::Upload(const void* data, size_t size)
{
	// Would issue a blit/replace region command in real implementation
}

#endif // USE_METAL
