#ifndef RENDERING_IRENDERBACKEND_H
#define RENDERING_IRENDERBACKEND_H

class IRenderBackend {
public:
virtual ~IRenderBackend() = default;

virtual bool HasExtension(const char* ext) const = 0; // backend extension check
virtual void GetIntegerv(unsigned int param, int* value) const = 0; // integer query
virtual bool GetVideoMemoryInfo(int* memInfo) const = 0; // VRAM info

static IRenderBackend& Get(); // global access
};

#endif // RENDERING_IRENDERBACKEND_H
