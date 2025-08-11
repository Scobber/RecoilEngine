/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#include "MetalBuffer.h"
#include <cstring>

/**
 * @brief MetalBuffer implementation
 *
 * The GL VBO class performs explicit bind/unbind and allocation calls.
 * Metal buffers are created directly from the device and remain mapped,
 * so most functions become thin wrappers or no-ops.
 */
MetalBuffer::MetalBuffer() = default;
MetalBuffer::~MetalBuffer() { Delete(); }

void MetalBuffer::Generate(id<MTLDevice> device, size_t size)
{
	Delete();
	bufSize = size;
	buffer = nullptr; // device->newBufferWithLength would go here
}

void MetalBuffer::Delete()
{
	if (buffer == nullptr)
		return;

	buffer = nullptr; // release in real implementation
	bufSize = 0;
}

void* MetalBuffer::MapBuffer()
{
	// Metal buffers are persistently mapped; return raw contents.
	return nullptr; // buffer->contents in real implementation
}

void MetalBuffer::UnmapBuffer()
{
	// No action required for Metal; kept for API symmetry with GL.
}

void MetalBuffer::SetBufferSubData(size_t offset, const void* data, size_t size)
{
	// Mirrors glBufferSubData; writes directly into the mapped region.
	std::memcpy(static_cast<char*>(MapBuffer()) + offset, data, size);
}

#endif // USE_METAL
