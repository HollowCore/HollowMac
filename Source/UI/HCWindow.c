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
void HCWindowDrawRect(id self, SEL _cmd, NSRect rect);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowRef HCWindowCreate(HCInteger width, HCInteger height) {
    HCWindowRef self = calloc(sizeof(HCWindow), 1);
    HCWindowInit(self, width, height);
    return self;
}

void HCWindowInit(void* memory, HCInteger width, HCInteger height) {
    // Create window
    id window = HCObjCSendIdMessageVoid((id)objc_getClass("NSWindow"), sel_getUid("alloc"));
    window = HCObjCSendIdMessageNSRectIntIntBool(
        window,
        sel_getUid("initWithContentRect:styleMask:backing:defer:"),
        (NSRect){ 0, 0, width, height },
        NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask,
        0,
        false
    );
    
    // Create window content view
    HCViewRef contentView = HCViewCreate(width, height);
    HCObjCSendVoidMessageId(window, sel_getUid("setContentView:"), contentView->view);
    
    // Initialize window object
    HCObjectInit(memory);
    HCWindowRef self = memory;
    self->base.type = HCWindowType;
    self->width = width;
    self->height = height;
    self->window = window;
    self->contentView = contentView;
}

void HCWindowDestroy(HCWindowRef self) {
    HCRelease(self->contentView);
    HCObjCSendVoidMessageVoid(self->window, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCWindowIsEqual(HCWindowRef self, HCWindowRef other) {
    return
        HCIntegerIsEqual(self->width, other->width) &&
        HCIntegerIsEqual(self->height, other->height) &&
        HCIntegerIsEqual((HCInteger)self->window, (HCInteger)other->window) &&
        HCIsEqual(self->contentView, other->contentView);
}

HCInteger HCWindowHashValue(HCWindowRef self) {
    return
        HCIntegerHashValue(self->width) ^
        HCIntegerHashValue(self->height) ^
        HCIntegerHashValue((HCInteger)self->window) ^
        HCHashValue(self->contentView);
}

void HCWindowPrint(HCWindowRef self, FILE* stream) {
    fprintf(stream, "<%s@%p,size:%lix%li>", self->base.type->name, self, (long)self->width, (long)self->height);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCWindowWidth(HCWindowRef self) {
    return self->width;
}

HCInteger HCWindowHeight(HCWindowRef self) {
    return self->height;
}

HCViewRef HCWindowContentView(HCWindowRef self) {
    return self->contentView;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowDisplay(HCWindowRef self) {
    HCObjCSendVoidMessageVoid(self->window, sel_getUid("becomeFirstResponder"));
    HCObjCSendVoidMessageId(self->window, sel_getUid("makeKeyAndOrderFront:"), NULL);
}
