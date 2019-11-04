//
//  HCWindow.c
//  Test
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCWindow_Internal.h"
#include "HCView_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCWindowTypeDataInstance = {
    .base = {
        .ancestor = (HCType)&HCObjectTypeDataInstance,
        .name = "HCWindow",
    },
    .isEqual = (void*)HCWindowIsEqual,
    .hashValue = (void*)HCWindowHashValue,
    .print = (void*)HCWindowPrint,
    .destroy = (void*)HCWindowDestroy,
};
HCType HCWindowType = (HCType)&HCWindowTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowDrawRect(id self, SEL cmd, CGRect rect);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowRef HCWindowCreate() {
    HCWindowRef self = calloc(sizeof(HCWindow), 1);
    HCWindowInit(self);
    return self;
}

void HCWindowInit(void* memory) {
    // Create window
    id window = HCObjcSendIdMessageVoid((id)objc_getClass("NSWindow"), sel_getUid("alloc"));
    window = HCObjcSendIdMessageCGRectIntIntBool(
        window,
        sel_getUid("initWithContentRect:styleMask:backing:defer:"),
        (CGRect){ {0, 0}, {720, 480} },
        NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask,
        0,
        false
    );
    
    // Create window content view
    HCViewRef contentView = HCViewCreate();
    HCObjcSendVoidMessageId(window, sel_getUid("setContentView:"), contentView->nsView);
    
    // Initialize window object
    HCObjectInit(memory);
    HCWindowRef self = memory;
    self->base.type = HCWindowType;
    self->nsWindow = window;
    self->contentView = contentView;
}

void HCWindowDestroy(HCWindowRef self) {
    HCRelease(self->contentView);
    HCObjcSendRelease(self->nsWindow);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCWindowIsEqual(HCWindowRef self, HCWindowRef other) {
    return
        HCIntegerIsEqual((HCInteger)self->nsWindow, (HCInteger)other->nsWindow) &&
        HCIsEqual(self->contentView, other->contentView);
}

HCInteger HCWindowHashValue(HCWindowRef self) {
    return
        HCIntegerHashValue((HCInteger)self->nsWindow) ^
        HCHashValue(self->contentView);
}

void HCWindowPrint(HCWindowRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowCoordinateLocation HCWindowOriginLocation(HCWindowRef self) {
    return HCWindowCoordinateLocationLowerLeft;
}

HCPoint HCWindowCenter(HCWindowRef self) {
    HCRectangle frame = HCWindowFrame(self);
    return HCPointMake(HCRectangleMidX(frame), HCRectangleMidY(frame));
}

void HCWindowSetCenter(HCWindowRef self, HCPoint center) {
    HCRectangle frame = HCWindowFrame(self);
    HCReal dx = center.x - HCRectangleMidX(frame);
    HCReal dy = center.y - HCRectangleMidY(frame);
    frame = HCRectangleOffset(frame, dx, dy);
    HCWindowSetFrame(self, frame);
}

HCPoint HCWindowOrigin(HCWindowRef self) {
    HCRectangle frame = HCWindowFrame(self);
    return frame.origin;
}

void HCWindowSetOrigin(HCWindowRef self, HCPoint origin) {
    HCRectangle frame = HCWindowFrame(self);
    frame.origin = origin;
    HCWindowSetFrame(self, frame);
}

HCSize HCWindowSize(HCWindowRef self) {
    HCRectangle frame = HCWindowFrame(self);
    return frame.size;
}

void HCWindowSetSize(HCWindowRef self, HCSize size) {
    HCRectangle frame = HCWindowFrame(self);
    frame.size = size;
    HCWindowSetFrame(self, frame);
}

HCRectangle HCWindowFrame(HCWindowRef self) {
    CGRect frame = HCObjcSendCGRectMessageVoid(self->nsWindow, sel_getUid("frame"));
    return HCRectangleMakeWithCGRect(frame);
}

void HCWindowSetFrame(HCWindowRef self, HCRectangle frame) {
    HCObjcSendVoidMessageCGRectBool(self->nsWindow, sel_getUid("setFrame:display:"), CGRectMakeWithHCRectangle(frame), true);
}

HCViewRef HCWindowContentView(HCWindowRef self) {
    return self->contentView;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowDisplay(HCWindowRef self) {
    HCObjcSendVoidMessageVoid(self->nsWindow, sel_getUid("becomeFirstResponder"));
    HCObjcSendVoidMessageId(self->nsWindow, sel_getUid("makeKeyAndOrderFront:"), NULL);
}
