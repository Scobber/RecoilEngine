/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifdef USE_METAL

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

// Owns the Metal device and command queue for the lifetime of the app.
static id<MTLDevice> metalDevice = nil;        // released in DestroyMetalContext
static id<MTLCommandQueue> metalQueue = nil;   // released in DestroyMetalContext

extern "C" void InitMetalContext(CAMetalLayer* layer)
{
        // Device is created once and retained by this module
        metalDevice = MTLCreateSystemDefaultDevice();
        metalQueue = [metalDevice newCommandQueue];

        // Layer keeps an unretained reference; SDL owns the layer
        [layer setDevice:metalDevice];
}

extern "C" void DestroyMetalContext()
{
        // Release objects according to Objective-C ownership rules
        [metalQueue release];
        [metalDevice release];

        metalQueue = nil;
        metalDevice = nil;
}

#endif
