/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#ifdef __OBJC__
#import <Metal/Metal.h>
#else
class MTLTexture; class MTLRenderPassDescriptor;
template <typename T> using id = T*;
#endif

/**
 * @brief MetalRenderTarget
 *
 * Thin wrapper around an MTLRenderPassDescriptor mirroring the GL FBO
 * interface used by higher level code.
 */
class MetalRenderTarget {
public:
	MetalRenderTarget();
	~MetalRenderTarget();

	bool Init();
	void Release();

	bool IsValid() const { return passDesc != nullptr; }
	void Bind() const {}
	void Unbind() const {}

	void AttachTexture(id<MTLTexture> tex, bool depth);

private:
	id<MTLRenderPassDescriptor> passDesc = nullptr;
	id<MTLTexture> colorTex = nullptr;
	id<MTLTexture> depthTex = nullptr;
};

#endif // USE_METAL
