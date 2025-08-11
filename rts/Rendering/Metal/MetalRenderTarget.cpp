/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#include "MetalRenderTarget.h"
#include <cassert>

/**
 * @brief MetalRenderTarget implementation
 *
 * GL's FBO allows explicit binding and status checks.  Metal relies on
 * MTLRenderPassDescriptor and encoders; this wrapper stores the descriptor
 * and keeps a similar API for the engine.
 */
MetalRenderTarget::MetalRenderTarget() = default;
MetalRenderTarget::~MetalRenderTarget() { Release(); }

bool MetalRenderTarget::Init()
{
        Release();
#ifdef __OBJC__
        passDesc = [MTLRenderPassDescriptor renderPassDescriptor];
#else
        passDesc = nullptr;
#endif
        return true;
}

void MetalRenderTarget::Release()
{
        passDesc = nullptr;
        colorTex = nullptr;
        depthTex = nullptr;
        activeEncoder = nullptr;
}

id<MTLRenderCommandEncoder> MetalRenderTarget::Begin(id<MTLCommandBuffer> buf)
{
#ifdef __OBJC__
        assert(passDesc != nil);
        assert(activeEncoder == nil);

        // ensure attachments are valid before creating encoder
        if (colorTex != nil) {
                MTLRenderPassColorAttachmentDescriptor* att = [passDesc.colorAttachments objectAtIndexedSubscript:0];
                att.texture = colorTex;
                att.loadAction = MTLLoadActionClear;
                att.storeAction = MTLStoreActionStore;
#ifndef NDEBUG
                assert(att.loadAction != MTLLoadActionDontCare);
                assert(att.storeAction != MTLStoreActionDontCare);
#endif
        }
        if (depthTex != nil) {
                MTLRenderPassDepthAttachmentDescriptor* datt = passDesc.depthAttachment;
                datt.texture = depthTex;
                datt.loadAction = MTLLoadActionClear;
                datt.storeAction = MTLStoreActionStore;
#ifndef NDEBUG
                assert(datt.loadAction != MTLLoadActionDontCare);
                assert(datt.storeAction != MTLStoreActionDontCare);
#endif
        }

        activeEncoder = [buf renderCommandEncoderWithDescriptor:passDesc];
        return activeEncoder;
#else
        return nullptr;
#endif
}

void MetalRenderTarget::End(id<MTLCommandBuffer> buf)
{
#ifdef __OBJC__
        if (activeEncoder != nil) {
                [activeEncoder endEncoding];
                activeEncoder = nil;
        }
        // commit the command buffer when provided
        if (buf != nil)
                [buf commit];
#endif
}

void MetalRenderTarget::AttachTexture(id<MTLTexture> tex, bool depth)
{
        if (depth)
                depthTex = tex;
        else
                colorTex = tex;

#ifdef __OBJC__
        if (passDesc != nil) {
                if (depth) {
                        MTLRenderPassDepthAttachmentDescriptor* datt = passDesc.depthAttachment;
                        datt.texture = tex;
                        datt.loadAction = MTLLoadActionClear;
                        datt.storeAction = MTLStoreActionStore;
#ifndef NDEBUG
                        assert(datt.loadAction != MTLLoadActionDontCare);
                        assert(datt.storeAction != MTLStoreActionDontCare);
#endif
                } else {
                        MTLRenderPassColorAttachmentDescriptor* att = [passDesc.colorAttachments objectAtIndexedSubscript:0];
                        att.texture = tex;
                        att.loadAction = MTLLoadActionClear;
                        att.storeAction = MTLStoreActionStore;
#ifndef NDEBUG
                        assert(att.loadAction != MTLLoadActionDontCare);
                        assert(att.storeAction != MTLStoreActionDontCare);
#endif
                }
        }
#endif
}

#endif // USE_METAL
