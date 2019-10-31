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
        .ancestor = &HCObjectTypeDataInstance.base,
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
void HCWindowDrawRect(id self, SEL _cmd, CGRect rect);

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
        (CGRect){ 0, 0, 720, 480 },
        NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask,
        0,
        false
    );
    
    // Create window content view
    HCViewRef contentView = HCViewCreate();
    HCViewSetFrame(contentView, HCRectangleMake(HCPointMake(0.0, 0.0), HCSizeMake(720.0, 480.0)));
    HCObjcSendVoidMessageId(window, sel_getUid("setContentView:"), contentView->view);
    
    // Initialize window object
    HCObjectInit(memory);
    HCWindowRef self = memory;
    self->base.type = HCWindowType;
    self->window = window;
    self->contentView = contentView;
}

void HCWindowDestroy(HCWindowRef self) {
    HCRelease(self->contentView);
    HCObjcSendVoidMessageVoid(self->window, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCWindowIsEqual(HCWindowRef self, HCWindowRef other) {
    return
        HCIntegerIsEqual((HCInteger)self->window, (HCInteger)other->window) &&
        HCIsEqual(self->contentView, other->contentView);
}

HCInteger HCWindowHashValue(HCWindowRef self) {
    return
        HCIntegerHashValue((HCInteger)self->window) ^
        HCHashValue(self->contentView);
}

void HCWindowPrint(HCWindowRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCViewRef HCWindowContentView(HCWindowRef self) {
    return self->contentView;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowDisplay(HCWindowRef self) {
    HCObjcSendVoidMessageVoid(self->window, sel_getUid("becomeFirstResponder"));
    HCObjcSendVoidMessageId(self->window, sel_getUid("makeKeyAndOrderFront:"), NULL);
}
