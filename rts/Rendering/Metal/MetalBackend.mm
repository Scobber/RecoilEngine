#ifdef USE_METAL
#import <Metal/Metal.h>

#include "Rendering/IRenderBackend.h"

class MetalBackend : public IRenderBackend {
public:
    bool HasExtension(const char* /*ext*/) const override { return false; }

    void GetIntegerv(unsigned int /*param*/, int* value) const override {
        if (value)
            *value = 0;
    }

    bool GetVideoMemoryInfo(int* memInfo) const override {
        id<MTLDevice> device = MTLCreateSystemDefaultDevice();
        if (!device)
            return false;
        uint64_t vram = [device recommendedMaxWorkingSetSize];
        memInfo[0] = static_cast<int>(vram / (1024 * 1024));
        memInfo[1] = memInfo[0];
        return true;
    }

    static MetalBackend& Instance() {
        static MetalBackend backend;
        return backend;
    }
};

IRenderBackend& IRenderBackend::Get() {
    return MetalBackend::Instance();
}
#endif // USE_METAL
