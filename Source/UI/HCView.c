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
const HCViewTypeData HCViewTypeDataInstance = {
    .base = {
        .base = {
            .ancestor = &HCObjectTypeDataInstance.base,
            .name = "HCView",
        },
        .isEqual = (void*)HCViewIsEqual,
        .hashValue = (void*)HCViewHashValue,
        .print = (void*)HCViewPrint,
        .destroy = (void*)HCViewDestroy,
    }
};
HCType HCViewType = &HCViewTypeDataInstance.base.base;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCViewRef HCViewCreate() {
    // Create NSView
    id nsView = HCObjcSendIdMessageVoid((id)objc_getClass("NSView"), sel_getUid("alloc"));
    nsView = HCObjcSendIdMessageVoid(
        nsView,
        sel_getUid("init")
    );
    HCObjcSendVoidMessageBool(nsView, sel_getUid("setWantsLayer:"), true);
    
    // Create HCView
    HCViewRef hcView = HCViewCreateWithView(nsView);
    HCObjcSendVoidMessageVoid(hcView->view, sel_getUid("release"));
    return hcView;
}

HCViewRef HCViewCreateWithView(id view) {
    HCViewRef self = calloc(sizeof(HCView), 1);
    HCViewInit(self, view);
    return self;
}

void HCViewInit(void* memory, id view) {
    // Initialize view object
    HCObjectInit(memory);
    HCViewRef self = memory;
    self->base.type = HCViewType;
    self->view = HCObjcSendIdMessageVoid(view,sel_getUid("retain"));
}

void HCViewDestroy(HCViewRef self) {
    HCObjcSendVoidMessageVoid(self->view, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCViewIsEqual(HCViewRef self, HCViewRef other) {
    return HCIntegerIsEqual((HCInteger)self->view, (HCInteger)other->view);
}

HCInteger HCViewHashValue(HCViewRef self) {
    return HCIntegerHashValue((HCInteger)self->view);
}

void HCViewPrint(HCViewRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCViewCenter(HCViewRef self) {
    HCRectangle frame = HCViewFrame(self);
    return HCPointMake(HCRectangleMidX(frame), HCRectangleMidY(frame));
}

void HCViewSetCenter(HCViewRef self, HCPoint center) {
    HCRectangle frame = HCViewFrame(self);
    HCReal dx = center.x - HCRectangleMidX(frame);
    HCReal dy = center.y - HCRectangleMidY(frame);
    frame = HCRectangleOffset(frame, dx, dy);
    HCViewSetFrame(self, frame);
}

HCSize HCViewSize(HCViewRef self) {
    CGRect bounds = HCObjcSendCGRectMessageVoid(self->view, sel_getUid("bounds"));
    return HCSizeMakeWithCGSize(bounds.size);
}

void HCViewSetSize(HCViewRef self, HCSize size) {
    HCObjcSendVoidMessageCGRect(self->view, sel_getUid("setBounds:"), CGRectMakeWithHCRectangle(HCRectangleMake(HCPointZero, size)));
}

HCRectangle HCViewFrame(HCViewRef self) {
    CGRect frame = HCObjcSendCGRectMessageVoid(self->view, sel_getUid("frame"));
    return HCRectangleMakeWithCGRect(frame);
}

void HCViewSetFrame(HCViewRef self, HCRectangle frame) {
    HCObjcSendVoidMessageCGRect(self->view, sel_getUid("setFrame:"), CGRectMakeWithHCRectangle(frame));
}

HCColor HCViewBackgroundColor(HCViewRef self) {
    id layer = HCObjcSendIdMessageVoid(self->view, sel_getUid("layer"));
    id backgroundCGColor = HCObjcSendIdMessageVoid(layer, sel_getUid("backgroundColor"));
    id backgroundNSColor = HCObjcSendIdMessageId((id)objc_getClass("NSColor"), sel_getUid("colorWithCGColor:"), backgroundCGColor);
    HCColor color = HCColorMakeWithNSColor(backgroundNSColor);
    HCObjcSendVoidMessageVoid(backgroundNSColor, sel_getUid("release"));
    return color;
}

void HCViewSetBackgroundColor(HCViewRef self, HCColor color) {
    id backgroundNSColor = NSColorAllocInitWithHCColor(color);
    id backgroundCGColor = HCObjcSendIdMessageVoid(backgroundNSColor, sel_getUid("CGColor"));
    id layer = HCObjcSendIdMessageVoid(self->view, sel_getUid("layer"));
    HCObjcSendVoidMessageId(layer, sel_getUid("setBackgroundColor:"), backgroundCGColor);
    HCObjcSendVoidMessageVoid(backgroundNSColor, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Related Views
//----------------------------------------------------------------------------------------------------------------------------------
HCViewRef HCViewParentViewRetained(HCViewRef self) {
    id parent = HCObjcSendIdMessageVoid(self->view, sel_getUid("superview"));
    return HCViewCreateWithView(parent);
}

HCListRef HCViewChildViewsRetained(HCViewRef self) {
    HCListRef childViews = HCListCreate();
    id subviews = HCObjcSendIdMessageVoid(self->view, sel_getUid("subviews"));
    NSInteger count = HCObjcSendNSIntegerMessageVoid(subviews, sel_getUid("count"));
    for (NSInteger index = 0; index < count; index++) {
        id subview = HCObjcSendIdMessageNSInteger(subviews, sel_getUid("objectAtIndex:"), index);
        HCListAddObjectReleased(childViews, HCViewCreateWithView(subview));
    }
    return childViews;
}

HCInteger HCViewChildViewCount(HCViewRef self) {
    id subviews = HCObjcSendIdMessageVoid(self->view, sel_getUid("subviews"));
    return HCObjcSendNSIntegerMessageVoid(subviews, sel_getUid("count"));
}

HCViewRef HCViewChildViewAtIndexRetained(HCViewRef self, HCInteger index) {
    id subviews = HCObjcSendIdMessageVoid(self->view, sel_getUid("subviews"));
    id subview = HCObjcSendIdMessageNSInteger(subviews, sel_getUid("objectAtIndex:"), index);
    return HCViewCreateWithView(subview);
}

void HCViewAddChildView(HCViewRef self, HCViewRef child) {
    HCObjcSendVoidMessageId(self->view, sel_getUid("addSubview:"), child->view);
}

void HCViewRemoveChildView(HCViewRef self, HCInteger index) {
    id subviews = HCObjcSendIdMessageVoid(self->view, sel_getUid("subviews"));
    id subview = HCObjcSendIdMessageNSInteger(subviews, sel_getUid("objectAtIndex:"), index);
    HCObjcSendVoidMessageVoid(subview, sel_getUid("removeFromSuperview"));
}
