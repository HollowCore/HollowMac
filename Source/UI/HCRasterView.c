//
//  HCRasterView.c
//  Controls
//
//  Created by Matt Stoker on 11/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCRasterView_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCViewTypeData HCRasterViewTypeDataInstance = {
    .base = {
        .base = {
            .ancestor = (HCType)&HCViewTypeDataInstance,
            .name = "HCRasterView",
        },
        .isEqual = (void*)HCViewIsEqual,
        .hashValue = (void*)HCViewHashValue,
        .print = (void*)HCRasterViewPrint,
        .destroy = (void*)HCRasterViewDestroy,
    },
};
HCType HCRasterViewType = (HCType)&HCRasterViewTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Put these into type struct
Class g_RasterViewSubclass = NULL;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterViewRef HCRasterViewCreate() {
    HCRasterViewRef self = calloc(sizeof(HCRasterView), 1);
    HCRasterViewInit(self);
    return self;
}

void HCRasterViewInit(void* memory) {
    // Register NSView subclass
    // TODO: Multi-thread safe
    if (g_RasterViewSubclass == NULL) {
        g_RasterViewSubclass = objc_allocateClassPair((Class)objc_getClass("NSView"), "RasterView", 0);
        class_addMethod(g_RasterViewSubclass, sel_getUid("drawRect:"), (IMP)HCRasterViewDrawRect, "v@:{CGRect={CGPoint=dd}{CGSize=dd}}");
        class_addMethod(g_RasterViewSubclass, sel_getUid("mouseDown:"), (IMP)HCRasterViewMouseDownEvent, "v@:@");
        class_addMethod(g_RasterViewSubclass, sel_getUid("mouseDragged:"), (IMP)HCRasterViewMouseMovedEvent, "v@:@");
        class_addMethod(g_RasterViewSubclass, sel_getUid("mouseUp:"), (IMP)HCRasterViewMouseUpEvent, "v@:@");
        class_addIvar(g_RasterViewSubclass, "hcRasterView", sizeof(uint64_t), log2(sizeof(uint64_t)), "Q");
        objc_registerClassPair(g_RasterViewSubclass);
    }
    
    // Create NSView subclass
    id nsView = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)g_RasterViewSubclass, sel_getUid("alloc")), sel_getUid("init"));
    HCObjcSendVoidMessageBool(nsView, sel_getUid("setWantsLayer:"), true);
    HCObjcSendVoidMessageBool(nsView, sel_getUid("setFlipped:"), true);
    
    // Initialize view object
    HCViewInit(memory, nsView);
    HCRasterViewRef self = memory;
    self->base.base.type = HCRasterViewType;
    
    // Put self into NSView for callbacks
    *(HCRasterViewRef*)((uint8_t*)nsView + ivar_getOffset(class_getInstanceVariable(g_RasterViewSubclass, "hcRasterView"))) = self;
    
    // Cleanup
    HCObjcSendRelease(nsView);
}

void HCRasterViewDestroy(HCRasterViewRef self) {
    (void)self; // Unused
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterViewPrint(HCRasterViewRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->base.base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterViewRasterRetained(HCRasterViewRef self) {
    (void)self; // Unused
    // TODO: This
    return NULL;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Events
//----------------------------------------------------------------------------------------------------------------------------------

HCRasterViewDrawFunction HCRasterViewDrawCallback(HCRasterViewRef self) {
    return self->drawCallback;
}

void HCRasterViewSetDrawCallback(HCRasterViewRef self, HCRasterViewDrawFunction callback, void* context) {
    self->drawCallback = callback;
    self->drawContext = context;
}

HCRasterViewMouseEventFunction HCRasterViewMouseDownCallback(HCRasterViewRef self) {
    return self->mouseDownCallback;
}

void HCRasterViewSetMouseDownCallback(HCRasterViewRef self, HCRasterViewMouseEventFunction callback, void* context) {
    self->mouseDownCallback = callback;
    self->mouseDownContext = context;
}

HCRasterViewMouseEventFunction HCRasterViewMouseMovedCallback(HCRasterViewRef self) {
    return self->mouseMovedCallback;
}

void HCRasterViewSetMouseMovedCallback(HCRasterViewRef self, HCRasterViewMouseEventFunction callback, void* context) {
    self->mouseMovedCallback = callback;
    self->mouseMovedContext = context;
}

HCRasterViewMouseEventFunction HCRasterViewMouseUpCallback(HCRasterViewRef self) {
    return self->mouseUpCallback;
}

void HCRasterViewSetMouseUpCallback(HCRasterViewRef self, HCRasterViewMouseEventFunction callback, void* context) {
    self->mouseUpCallback = callback;
    self->mouseUpContext = context;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterViewDraw(HCRasterViewRef self) {
    HCRasterViewDrawRect(self->base.nsView, sel_getUid("drawRect:"), HCObjcSendCGRectMessageVoid(self->base.nsView, sel_getUid("bounds")));
}

void HCRasterViewInvalidate(HCRasterViewRef self) {
    if (!HCViewHasParentView((HCViewRef)self)) {
        HCRasterViewDraw(self);
        return;
    }
    HCObjcSendVoidMessageCGRect(self->base.nsView, sel_getUid("setNeedsDisplayInRect:"), CGRectMakeWithHCRectangle(HCViewFrame((HCViewRef)self)));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterViewDrawRect(id nsView, SEL cmd, CGRect dirtyRect) {
    (void)cmd; (void)dirtyRect; // Unused
    
//    HCObjcSendVoidMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSColor"), sel_getUid("redColor")), sel_getUid("set"));
//    HCObjcSendVoidMessageVoid(HCObjcSendIdMessageCGRect((id)objc_getClass("NSBezierPath"), sel_getUid("bezierPathWithOvalInRect:"), dirtyRect), sel_getUid("fill"));

    HCRasterViewRef self = *(HCRasterViewRef*)((uint8_t*)nsView + ivar_getOffset(class_getInstanceVariable(g_RasterViewSubclass, "hcRasterView")));
    HCInteger width = (HCInteger)ceil(HCViewSize((HCViewRef)self).width);
    HCInteger height = (HCInteger)ceil(HCViewSize((HCViewRef)self).height);

    if (width * height == 0) {
        return;
    }

    HCRasterRef raster = HCRasterCreate(width, height);
    if (self->drawCallback != NULL) {
        self->drawCallback(self->drawContext, self, raster);
    }

    id nsBitmapImageRep = HCObjcSendIdMessageVoid((id)objc_getClass("NSBitmapImageRep"), sel_getUid("alloc"));
    typedef id (*HCObjcBitmapImageRepInitFunction)(id, SEL, unsigned char**, NSInteger, NSInteger, NSInteger, NSInteger, BOOL, BOOL, id, NSInteger, NSInteger);
    static HCObjcBitmapImageRepInitFunction HCObjcSendBitmapImageRepInit = (HCObjcBitmapImageRepInitFunction)objc_msgSend;
    SEL NSBitmapImageRepInitFunctionSEL = sel_getUid("initWithBitmapDataPlanes:pixelsWide:pixelsHigh:bitsPerSample:samplesPerPixel:hasAlpha:isPlanar:colorSpaceName:bytesPerRow:bitsPerPixel:");
    HCStringRef colorSpace = HCStringCreateWithCString("NSCalibratedRGBColorSpace");
    id nsColorSpace = NSStringAllocInitWithHCString(colorSpace);
    nsBitmapImageRep = HCObjcSendBitmapImageRepInit(nsBitmapImageRep, NSBitmapImageRepInitFunctionSEL,
        NULL,
        width,
        height,
        8,
        4,
        true,
        false,
        nsColorSpace,
        width*4,
        32);
    void* bitmapData = HCObjcSendIdMessageVoid(nsBitmapImageRep, sel_getUid("bitmapData"));
    HCRasterExtractARGB8888(raster, bitmapData);
    
    id nsImage = HCObjcSendIdMessageVoid((id)objc_getClass("NSImage"), sel_getUid("alloc"));
    nsImage = HCObjcSendIdMessageCGSize(nsImage, sel_getUid("initWithSize:"), (CGSize){width, height});
    HCObjcSendVoidMessageId(nsImage, sel_getUid("addRepresentation:"), nsBitmapImageRep);

    HCObjcSendVoidMessageCGRect(nsImage, sel_getUid("drawInRect:"), (CGRect){ {0, 0}, CGSizeMakeWithHCSize(HCViewSize((HCViewRef)self)) });

    HCObjcSendRelease(nsImage);
    HCObjcSendRelease(nsBitmapImageRep);
    HCObjcSendRelease(nsColorSpace);
    HCRelease(colorSpace);
    HCRelease(raster);
}

void HCRasterViewMouseDownEvent(id nsView, SEL cmd, id event) {
    (void)cmd; // Unused
    HCRasterViewRef self = *(HCRasterViewRef*)((uint8_t*)nsView + ivar_getOffset(class_getInstanceVariable(g_RasterViewSubclass, "hcRasterView")));
    
    CGPoint windowPoint = HCObjcSendCGPointMessageVoid(event, sel_getUid("locationInWindow"));
    CGPoint viewPoint = HCObjcSendCGPointMessageCGPointId(nsView, sel_getUid("convertPoint:fromView:"), windowPoint, NULL);
    HCPoint location = HCPointMakeWithCGPoint(viewPoint);
    
    if (self->mouseDownCallback != NULL) {
        self->mouseDownCallback(self->mouseDownContext, self, location);
    }
}

void HCRasterViewMouseMovedEvent(id nsView, SEL cmd, id event) {
    (void)cmd; // Unused
    HCRasterViewRef self = *(HCRasterViewRef*)((uint8_t*)nsView + ivar_getOffset(class_getInstanceVariable(g_RasterViewSubclass, "hcRasterView")));
    
    CGPoint windowPoint = HCObjcSendCGPointMessageVoid(event, sel_getUid("locationInWindow"));
    CGPoint viewPoint = HCObjcSendCGPointMessageCGPointId(nsView, sel_getUid("convertPoint:fromView:"), windowPoint, NULL);
    HCPoint location = HCPointMakeWithCGPoint(viewPoint);
    
    if (self->mouseMovedCallback != NULL) {
        self->mouseMovedCallback(self->mouseMovedContext, self, location);
    }
}

void HCRasterViewMouseUpEvent(id nsView, SEL cmd, id event) {
    (void)cmd; // Unused
    HCRasterViewRef self = *(HCRasterViewRef*)((uint8_t*)nsView + ivar_getOffset(class_getInstanceVariable(g_RasterViewSubclass, "hcRasterView")));
    
    CGPoint windowPoint = HCObjcSendCGPointMessageVoid(event, sel_getUid("locationInWindow"));
    CGPoint viewPoint = HCObjcSendCGPointMessageCGPointId(nsView, sel_getUid("convertPoint:fromView:"), windowPoint, NULL);
    HCPoint location = HCPointMakeWithCGPoint(viewPoint);
    
    if (self->mouseUpCallback != NULL) {
        self->mouseUpCallback(self->mouseUpContext, self, location);
    }
}
