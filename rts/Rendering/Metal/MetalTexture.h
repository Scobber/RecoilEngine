/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#ifdef __OBJC__
#import <Metal/Metal.h>
#else
class MTLDevice; class MTLTexture;
template <typename T> using id = T*;
#endif

/**
 * @brief MetalTexture
 *
 * Rough equivalent of GL::Texture2D but wrapping a Metal texture handle.
 * Metal does not use explicit bind points so the interface is simplified.
 */
class MetalTexture {
public:
	MetalTexture();
	~MetalTexture();

	void Allocate(id<MTLDevice> device, size_t width, size_t height, MTLPixelFormat format);
	void Release();

	void Upload(const void* data, size_t size);

	id<MTLTexture> GetId() const { return texture; }

private:
	id<MTLTexture> texture = nullptr;
	size_t xsize = 0;
	size_t ysize = 0;
};

#endif // USE_METAL
