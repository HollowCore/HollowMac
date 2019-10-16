//
//  HCButton.c
//  Test
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCButton_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCButtonTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCButton",
    },
    .isEqual = (void*)HCButtonIsEqual,
    .hashValue = (void*)HCButtonHashValue,
    .print = (void*)HCButtonPrint,
    .destroy = (void*)HCButtonDestroy,
};
HCType HCButtonType = (HCType)&HCButtonTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
void HCButtonClickEvent(id eventReceiver);

// TODO: Put these into type struct
Class g_EventReceiverClass = NULL;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCButtonRef HCButtonCreate(HCInteger width, HCInteger height) {
    HCButtonRef self = calloc(sizeof(HCButton), 1);
    HCButtonInit(self, width, height);
    return self;
}

void HCButtonInit(void* memory, HCInteger width, HCInteger height) {
    // Register event receiver NSObject subclass
    // TODO: Multi-thread safe
    if (g_EventReceiverClass == NULL) {
        g_EventReceiverClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "EventReceiver", 0);
        class_addMethod(g_EventReceiverClass, sel_getUid("click:"), (IMP)HCButtonClickEvent, "v@:");
        objc_registerClassPair(g_EventReceiverClass);
    }
    
    // Create button
    id button = HCObjCSendIdMessageVoid((id)objc_getClass("NSButton"), sel_getUid("alloc"));
    button = HCObjCSendIdMessageNSRect(
        button,
        sel_getUid("initWithFrame:"),
        (NSRect){ 0, 0, width, height }
    );
    
    // Create event receiver
    id eventReceiver = HCObjCSendIdMessageVoid((id)g_EventReceiverClass, sel_getUid("alloc"));
    eventReceiver = HCObjCSendIdMessageVoid(eventReceiver, sel_getUid("init"));
    
    // Register button click callback
    HCObjCSendVoidMessageId(button, sel_getUid("setTarget:"), eventReceiver);
    HCObjCSendVoidMessageId(button, sel_getUid("setAction:"), (id)sel_getUid("click:"));
    
    // Initialize button object
    HCObjectInit(memory);
    HCButtonRef self = memory;
    self->base.type = HCButtonType;
    self->width = width;
    self->height = height;
    self->button = button;
    self->eventReceiver = eventReceiver;
}

void HCButtonDestroy(HCButtonRef self) {
    HCObjCSendVoidMessageVoid(self->eventReceiver, sel_getUid("release"));
    HCObjCSendVoidMessageVoid(self->button, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCButtonIsEqual(HCButtonRef self, HCButtonRef other) {
    return
        HCIntegerIsEqual(self->width, other->width) &&
        HCIntegerIsEqual(self->height, other->height) &&
        HCIntegerIsEqual((HCInteger)self->button, (HCInteger)other->button);
}

HCInteger HCButtonHashValue(HCButtonRef self) {
    return
        HCIntegerHashValue(self->width) ^
        HCIntegerHashValue(self->height) ^
        HCIntegerHashValue((HCInteger)self->button);
}

void HCButtonPrint(HCButtonRef self, FILE* stream) {
    fprintf(stream, "<%s@%p,size:%lix%li>", self->base.type->name, self, (long)self->width, (long)self->height);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCButtonWidth(HCButtonRef self) {
    return self->width;
}

HCInteger HCButtonHeight(HCButtonRef self) {
    return self->height;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
void HCButtonClickEvent(id eventReceiver) {
    printf("Clicked!\n");
}
