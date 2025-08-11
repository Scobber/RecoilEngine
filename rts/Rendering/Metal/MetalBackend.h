/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#pragma once

#ifdef USE_METAL

#include "Rendering/IRenderBackend.h"

#ifdef __OBJC__
#import <Metal/Metal.h>
#import <mach/mach.h>
#else
class MTLDevice;
template <typename T> using id = T*;
struct task_vm_info_data_t;
#endif

/**
 * Metal implementation of the generic render backend interface.
 * The backend exposes minimal queries required by the engine to gather
 * hardware information without relying on OpenGL entry points.
 */
class MetalRenderBackend final : public IRenderBackend {
public:
        MetalRenderBackend();

        bool HasExtension(const char* ext) const override;
        void GetIntegerv(unsigned int param, int* value) const override;
        bool GetVideoMemoryInfo(int* memInfo) const override;

private:
        id<MTLDevice> device;
};

#endif // USE_METAL
