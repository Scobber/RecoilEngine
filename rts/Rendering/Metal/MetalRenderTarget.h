/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#ifdef __OBJC__
#import <Metal/Metal.h>
#else
class MTLTexture; class MTLRenderPassDescriptor; class MTLCommandBuffer; class MTLRenderCommandEncoder;
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

        // start and end a render pass; wrappers around Metal encoders
        id<MTLRenderCommandEncoder> Begin(id<MTLCommandBuffer> buf);
        void End(id<MTLCommandBuffer> buf);

        void AttachTexture(id<MTLTexture> tex, bool depth);

private:
        id<MTLRenderPassDescriptor> passDesc = nullptr;
        id<MTLTexture> colorTex = nullptr;
        id<MTLTexture> depthTex = nullptr;
        id<MTLRenderCommandEncoder> activeEncoder = nullptr;
};

#endif // USE_METAL
