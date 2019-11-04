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
            .ancestor = (HCType)&HCObjectTypeDataInstance,
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
    id nsView = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSView"), sel_getUid("alloc")), sel_getUid("init"));
    HCObjcSendVoidMessageBool(nsView, sel_getUid("setWantsLayer:"), true);
    HCObjcSendVoidMessageBool(nsView, sel_getUid("setFlipped:"), true);
    
    // Create HCView
    HCViewRef view = HCViewCreateWithNSView(nsView);
    HCObjcSendRelease(view->nsView);
    return view;
}

HCViewRef HCViewCreateWithNSView(id nsView) {
    HCViewRef self = calloc(sizeof(HCView), 1);
    HCViewInit(self, nsView);
    return self;
}

void HCViewInit(void* memory, id nsView) {
    // Initialize view object
    HCObjectInit(memory);
    HCViewRef self = memory;
    self->base.type = HCViewType;
    self->nsView = HCObjcSendRetain(nsView);
}

void HCViewDestroy(HCViewRef self) {
    HCObjcSendRelease(self->nsView);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCViewIsEqual(HCViewRef self, HCViewRef other) {
    return HCIntegerIsEqual((HCInteger)self->nsView, (HCInteger)other->nsView);
}

HCInteger HCViewHashValue(HCViewRef self) {
    return HCIntegerHashValue((HCInteger)self->nsView);
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

HCPoint HCViewOrigin(HCViewRef self) {
    HCRectangle frame = HCViewFrame(self);
    return frame.origin;
}

void HCViewSetOrigin(HCViewRef self, HCPoint origin) {
    HCRectangle frame = HCViewFrame(self);
    frame.origin = origin;
    HCViewSetFrame(self, frame);
}

HCSize HCViewSize(HCViewRef self) {
    HCRectangle frame = HCViewFrame(self);
    return frame.size;
}

void HCViewSetSize(HCViewRef self, HCSize size) {
    HCRectangle frame = HCViewFrame(self);
    frame.size = size;
    HCViewSetFrame(self, frame);
}

HCRectangle HCViewFrame(HCViewRef self) {
    CGRect frame = HCObjcSendCGRectMessageVoid(self->nsView, sel_getUid("frame"));
    return HCRectangleMakeWithCGRect(frame);
}

void HCViewSetFrame(HCViewRef self, HCRectangle frame) {
    HCObjcSendVoidMessageCGRect(self->nsView, sel_getUid("setFrame:"), CGRectMakeWithHCRectangle(frame));
}

HCColor HCViewBackgroundColor(HCViewRef self) {
    id layer = HCObjcSendIdMessageVoid(self->nsView, sel_getUid("layer"));
    id backgroundCGColor = HCObjcSendIdMessageVoid(layer, sel_getUid("backgroundColor"));
    id backgroundNSColor = HCObjcSendIdMessageId((id)objc_getClass("NSColor"), sel_getUid("colorWithCGColor:"), backgroundCGColor);
    HCColor color = HCColorMakeWithNSColor(backgroundNSColor);
    HCObjcSendRelease(backgroundNSColor);
    return color;
}

void HCViewSetBackgroundColor(HCViewRef self, HCColor color) {
    id backgroundNSColor = NSColorAllocInitWithHCColor(color);
    id backgroundCGColor = HCObjcSendIdMessageVoid(backgroundNSColor, sel_getUid("CGColor"));
    id layer = HCObjcSendIdMessageVoid(self->nsView, sel_getUid("layer"));
    HCObjcSendVoidMessageId(layer, sel_getUid("setBackgroundColor:"), backgroundCGColor);
    HCObjcSendRelease(backgroundNSColor);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Related Views
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCViewHasParentView(HCViewRef self) {
    return HCObjcSendIdMessageVoid(self->nsView, sel_getUid("superview")) != NULL;
}

HCViewRef HCViewParentViewRetained(HCViewRef self) {
    id parent = HCObjcSendIdMessageVoid(self->nsView, sel_getUid("superview"));
    return parent == NULL ? NULL : HCViewCreateWithNSView(parent);
}

HCInteger HCViewChildViewCount(HCViewRef self) {
    id subviews = HCObjcSendIdMessageVoid(self->nsView, sel_getUid("subviews"));
    return HCObjcSendNSIntegerMessageVoid(subviews, sel_getUid("count"));
}

HCViewRef HCViewChildViewAtIndexRetained(HCViewRef self, HCInteger index) {
    id subviews = HCObjcSendIdMessageVoid(self->nsView, sel_getUid("subviews"));
    id subview = HCObjcSendIdMessageNSInteger(subviews, sel_getUid("objectAtIndex:"), index);
    return subview == NULL ? NULL : HCViewCreateWithNSView(subview);
}

void HCViewAddChildView(HCViewRef self, HCViewRef child) {
    HCObjcSendVoidMessageId(self->nsView, sel_getUid("addSubview:"), child->nsView);
}

void HCViewRemoveChildView(HCViewRef self, HCInteger index) {
    id subviews = HCObjcSendIdMessageVoid(self->nsView, sel_getUid("subviews"));
    id subview = HCObjcSendIdMessageNSInteger(subviews, sel_getUid("objectAtIndex:"), index);
    HCObjcSendVoidMessageVoid(subview, sel_getUid("removeFromSuperview"));
}
