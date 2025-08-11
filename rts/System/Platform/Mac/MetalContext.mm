/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <CoreVideo/CoreVideo.h>

// Owns the Metal device and command queue for the lifetime of the app.
static id<MTLDevice>      metalDevice = nil;     // released in DestroyMetalContext
static id<MTLCommandQueue> metalQueue = nil;     // released in DestroyMetalContext
static CAMetalLayer*       metalLayer = nil;     // weak reference owned by SDL view
static CVDisplayLinkRef    displayLink = nullptr;


extern "C" bool InitMetalContext(CAMetalLayer* layer, int width, int height)
{
metalDevice = MTLCreateSystemDefaultDevice();
if (metalDevice == nil) {
NSLog(@"[InitMetalContext] failed to create Metal device");
return false;
}
metalQueue = [metalDevice newCommandQueue];
if (metalQueue == nil) {
NSLog(@"[InitMetalContext] failed to create command queue");
[metalDevice release];
metalDevice = nil;
return false;
}

metalLayer = layer;
[metalLayer setDevice:metalDevice];
[metalLayer setPixelFormat:MTLPixelFormatBGRA8Unorm];
[metalLayer setFramebufferOnly:YES];
[metalLayer setDrawableSize:CGSizeMake(width, height)];

if (CVDisplayLinkCreateWithActiveCGDisplays(&displayLink) != kCVReturnSuccess) {
NSLog(@"[InitMetalContext] failed to create display link");
displayLink = nullptr;
} else {
CVDisplayLinkStart(displayLink);
}

return true;
}

extern "C" CAMetalDrawable* CreateDrawable()
{
if (metalLayer == nil)
return nil;
return [metalLayer nextDrawable];
}

extern "C" void SetMetalDrawableSize(int width, int height)
{
if (metalLayer != nil) {
[metalLayer setDrawableSize:CGSizeMake(width, height)];
}
}

extern "C" void DestroyMetalContext()
{
NSLog(@"[DestroyMetalContext] releasing Metal resources");
if (displayLink != nullptr) {
CVDisplayLinkStop(displayLink);
CVDisplayLinkRelease(displayLink);
displayLink = nullptr;
}

if (metalQueue != nil) {
[metalQueue release];
metalQueue = nil;
}
if (metalDevice != nil) {
[metalDevice release];
metalDevice = nil;
}
metalLayer = nil;
}

#endif
