#ifdef USE_METAL
#import <Metal/Metal.h>

/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */
#import <Metal/Metal.h>
#import <mach/mach.h>

#include "Rendering/IRenderBackend.h"
#include "MetalBackend.h"

// Fallback definitions for GL enums queried by the engine.  These values
// mirror the ones defined by the corresponding OpenGL extensions so that the
// rest of the engine can issue the same numeric queries regardless of the
// active backend.
#ifndef GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX
#define GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX 0x9048
#endif
#ifndef GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX
#define GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
#endif
#ifndef GL_VBO_FREE_MEMORY_ATI
#define GL_VBO_FREE_MEMORY_ATI 0x87FB
#endif
#ifndef GL_TEXTURE_FREE_MEMORY_ATI
#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
#endif

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


MetalRenderBackend::MetalRenderBackend()
{
        device = MTLCreateSystemDefaultDevice();
}

bool MetalRenderBackend::HasExtension(const char* /*ext*/) const
{
        // Metal does not expose string based extensions; feature queries are
        // performed through feature sets instead.  Return false to indicate
        // absence of any GL style extension.
        return false;
}

void MetalRenderBackend::GetIntegerv(unsigned int param, int* value) const
{
        if (value == nullptr || device == nil)
                return;

        // Only the subset of queries used by the engine are supported.
        switch (param) {
                case GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX: {
                        if ([device respondsToSelector:@selector(recommendedMaxWorkingSetSize)])
                                *value = int([device recommendedMaxWorkingSetSize] / 1024);
                        else
                                *value = 0;
                } break;

                case GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX:
                case GL_VBO_FREE_MEMORY_ATI:
                case GL_TEXTURE_FREE_MEMORY_ATI: {
                        size_t total = 0;
                        size_t used = 0;

                        if ([device respondsToSelector:@selector(recommendedMaxWorkingSetSize)])
                                total = [device recommendedMaxWorkingSetSize];

                        if ([device respondsToSelector:@selector(currentAllocatedSize)]) {
                                used = [device currentAllocatedSize];
                        } else {
                                task_vm_info_data_t vmInfo;
                                mach_msg_type_number_t count = TASK_VM_INFO_COUNT;
                                if (task_info(mach_task_self(), TASK_VM_INFO,
                                              (task_info_t)&vmInfo, &count) == KERN_SUCCESS) {
                                        used = vmInfo.phys_footprint;
                                }
                        }

                        const size_t freeBytes = (total > used) ? (total - used) : 0;
                        *value = int(freeBytes / 1024);
                } break;

                default:
                        *value = 0;
                        break;
        }
}

bool MetalRenderBackend::GetVideoMemoryInfo(int* memInfo) const
{
        if (memInfo == nullptr || device == nil)
                return false;

        size_t total = 0;
        size_t used = 0;

        if ([device respondsToSelector:@selector(recommendedMaxWorkingSetSize)])
                total = [device recommendedMaxWorkingSetSize];

        if ([device respondsToSelector:@selector(currentAllocatedSize)]) {
                used = [device currentAllocatedSize];
        } else {
                task_vm_info_data_t vmInfo;
                mach_msg_type_number_t count = TASK_VM_INFO_COUNT;
                if (task_info(mach_task_self(), TASK_VM_INFO,
                              (task_info_t)&vmInfo, &count) == KERN_SUCCESS) {
                        used = vmInfo.phys_footprint;
                }
        }

        const size_t freeBytes = (total > used) ? (total - used) : 0;
        memInfo[0] = int(total / 1024);
        memInfo[1] = int(freeBytes / 1024);
        return true;
}

#endif // USE_METAL

