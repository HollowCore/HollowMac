//
//  HCWindow.c
//  Test
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCWindow_Internal.h"
#include "../../HollowCore/Source/Core/HCCore.h"
#include <math.h>
#include <string.h>
#include <objc/message.h>
#include <objc/runtime.h>

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

// TODO: Put these into type struct
Class g_ViewClass = NULL;

extern void NSRectFill(NSRect aRect);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowRef HCWindowCreate(HCInteger width, HCInteger height) {
    HCWindowRef self = calloc(sizeof(HCWindow), 1);
    HCWindowInit(self, width, height);
    return self;
}

void HCWindowInit(void* memory, HCInteger width, HCInteger height) {
    // Register View class
    // TODO: Multi-thread safe
    if (g_ViewClass == NULL) {
        g_ViewClass = objc_allocateClassPair((Class)objc_getClass("NSView"), "View", 0);
        class_addMethod(g_ViewClass, sel_getUid("drawRect:"), (IMP)HCWindowDrawRect, "v@:");
        objc_registerClassPair(g_ViewClass);
    }
    
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
    id contentView = HCObjCSendIdMessageVoid((id)objc_getClass("View"), sel_getUid("alloc"));
    contentView = HCObjCSendIdMessageNSRect(
        contentView,
        sel_getUid("initWithFrame:"),
        (NSRect){ 0, 0, 200, 100 }
    );
    HCObjCSendVoidMessageId(window, sel_getUid("setContentView:"), contentView);
    
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
    HCObjCSendVoidMessageVoid(self->contentView, sel_getUid("release"));
    HCObjCSendVoidMessageVoid(self->window, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCWindowIsEqual(HCWindowRef self, HCWindowRef other) {
    if (self->width != other->width || self->height != other->height) {
        return false;
    }
    return true;
}

HCInteger HCWindowHashValue(HCWindowRef self) {
    return HCIntegerHashValue(self->width) ^ HCIntegerHashValue(self->height);
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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowDisplay(HCWindowRef self) {
    HCObjCSendVoidMessageVoid(self->window, sel_getUid("becomeFirstResponder"));
    HCObjCSendVoidMessageId(self->window, sel_getUid("makeKeyAndOrderFront:"), NULL);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowDrawRect(id self, SEL _cmd, NSRect rect) {
    id redColor = HCObjCSendIdMessageVoid((id)objc_getClass("NSColor"), sel_getUid("redColor"));
    
    NSRect rect1 = (NSRect){ 21, 21, 210, 210 };
    HCObjCSendVoidMessageVoid(redColor, sel_getUid("set"));
    NSRectFill(rect1);
}
