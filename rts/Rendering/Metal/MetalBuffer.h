/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#ifdef __OBJC__
#import <Metal/Metal.h>
#else
class MTLDevice; class MTLBuffer;
template <typename T> using id = T*;
#endif

/**
 * @brief MetalBuffer
 *
 * Mirrors the interface of GL's VBO but wraps a Metal buffer handle.
 * Unlike GL buffers, Metal buffers are persistently mapped so Map/Unmap
 * become no-ops.  This class only exposes the subset of the VBO API used by
 * higher level renderers.
 */
class MetalBuffer {
public:
	MetalBuffer();
	~MetalBuffer();

	void Generate(id<MTLDevice> device, size_t size);
	void Delete();

	void* MapBuffer();
	void UnmapBuffer();

	void SetBufferSubData(size_t offset, const void* data, size_t size);

	id<MTLBuffer> GetId() const { return buffer; }
	size_t GetSize() const { return bufSize; }

private:
	id<MTLBuffer> buffer = nullptr;
	size_t bufSize = 0;
};

#endif // USE_METAL
