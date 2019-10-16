//
//  HCView.c
//  Test
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCView_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCViewTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCView",
    },
    .isEqual = (void*)HCViewIsEqual,
    .hashValue = (void*)HCViewHashValue,
    .print = (void*)HCViewPrint,
    .destroy = (void*)HCViewDestroy,
};
HCType HCViewType = (HCType)&HCViewTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
void HCViewDrawRect(id self, SEL _cmd, NSRect rect);

// TODO: Put these into type struct
Class g_ViewClass = NULL;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCViewRef HCViewCreate(HCInteger width, HCInteger height) {
    HCViewRef self = calloc(sizeof(HCView), 1);
    HCViewInit(self, width, height);
    return self;
}

void HCViewInit(void* memory, HCInteger width, HCInteger height) {
    // Register NSView subclass
    // TODO: Multi-thread safe
    if (g_ViewClass == NULL) {
        g_ViewClass = objc_allocateClassPair((Class)objc_getClass("NSView"), "View", 0);
        class_addMethod(g_ViewClass, sel_getUid("drawRect:"), (IMP)HCViewDrawRect, "v@:");
        objc_registerClassPair(g_ViewClass);
    }
    
    // Create view
    id view = HCObjCSendIdMessageVoid((id)g_ViewClass, sel_getUid("alloc"));
    view = HCObjCSendIdMessageNSRect(
        view,
        sel_getUid("initWithFrame:"),
        (NSRect){ 0, 0, width, height }
    );
    
    // Initialize view object
    HCObjectInit(memory);
    HCViewRef self = memory;
    self->base.type = HCViewType;
    self->width = width;
    self->height = height;
    self->view = view;
    self->childViews = HCListCreate();
}

void HCViewDestroy(HCViewRef self) {
    HCRelease(self->childViews);
    HCObjCSendVoidMessageVoid(self->view, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCViewIsEqual(HCViewRef self, HCViewRef other) {
    return
        HCIntegerIsEqual(self->width, other->width) &&
        HCIntegerIsEqual(self->height, other->height) &&
        HCIntegerIsEqual((HCInteger)self->view, (HCInteger)other->view);
}

HCInteger HCViewHashValue(HCViewRef self) {
    return
        HCIntegerHashValue(self->width) ^
        HCIntegerHashValue(self->height) ^
        HCIntegerHashValue((HCInteger)self->view);
}

void HCViewPrint(HCViewRef self, FILE* stream) {
    fprintf(stream, "<%s@%p,size:%lix%li>", self->base.type->name, self, (long)self->width, (long)self->height);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCViewWidth(HCViewRef self) {
    return self->width;
}

HCInteger HCViewHeight(HCViewRef self) {
    return self->height;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCViewAddChildView(HCViewRef self, HCViewRef child) {
    HCListAddObject(self->childViews, child);
    HCObjCSendVoidMessageId(self->view, sel_getUid("addSubview:"), child->view);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
void HCViewDrawRect(id self, SEL _cmd, NSRect rect) {
    id color = HCObjCSendIdMessageVoid((id)objc_getClass("NSColor"), sel_getUid("blueColor"));
    
    NSRect rect1 = (NSRect){ 21, 21, 110, 110 };
    HCObjCSendVoidMessageVoid(color, sel_getUid("set"));
    NSRectFill(rect1);
}
