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
const HCViewTypeData HCButtonTypeDataInstance = {
    .base = {
        .base = {
            .ancestor = (HCType)&HCViewTypeDataInstance,
            .name = "HCButton",
        },
        .isEqual = (void*)HCViewIsEqual,
        .hashValue = (void*)HCViewHashValue,
        .print = (void*)HCButtonPrint,
        .destroy = (void*)HCButtonDestroy,
    },
};
HCType HCButtonType = (HCType)&HCButtonTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Put these into type struct
Class g_EventReceiverClass = NULL;
void HCButtonClickEvent(id eventReceiver, SEL _cmd, id sender);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCButtonRef HCButtonCreate() {
    // Register event receiver NSObject subclass
    // TODO: Multi-thread safe
    if (g_EventReceiverClass == NULL) {
        g_EventReceiverClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "EventReceiver", 0);
        class_addMethod(g_EventReceiverClass, sel_getUid("click:"), (IMP)HCButtonClickEvent, "v@:@");
        class_addIvar(g_EventReceiverClass, "hcButton", sizeof(uint64_t), log2(sizeof(uint64_t)), "Q");
        objc_registerClassPair(g_EventReceiverClass);
    }
    
    // Create button
    id button = HCObjcSendIdMessageVoid((id)objc_getClass("NSButton"), sel_getUid("alloc"));
    button = HCObjcSendIdMessageCGRect(
        button,
        sel_getUid("initWithFrame:"),
        (CGRect){ 0, 0, 0, 0 }
    );
    
    return HCButtonCreateWithView(button);
}

HCButtonRef HCButtonCreateWithView(id button) {
    HCButtonRef self = calloc(sizeof(HCButton), 1);
    HCButtonInit(self, button);
    return self;
}

void HCButtonInit(void* memory, id button) {
    // Create event receiver
    id eventReceiver = HCObjcSendIdMessageVoid((id)g_EventReceiverClass, sel_getUid("alloc"));
    eventReceiver = HCObjcSendIdMessageVoid(eventReceiver, sel_getUid("init"));
    
    // Register button click callback
    HCObjcSendVoidMessageId(button, sel_getUid("setTarget:"), eventReceiver);
    HCObjcSendVoidMessageId(button, sel_getUid("setAction:"), (id)sel_getUid("click:"));
    
    // Initialize button object
    HCViewInit(memory, button);
    HCButtonRef self = memory;
    self->base.base.type = HCButtonType;
    self->eventReceiver = eventReceiver;
    
    // Put self into event receiver for callbacks
    ptrdiff_t offset = ivar_getOffset(class_getInstanceVariable(g_EventReceiverClass, "hcButton"));
    *(HCButtonRef*)((uint8_t*)eventReceiver + offset) = self;
}

void HCButtonDestroy(HCButtonRef self) {
    HCObjcSendVoidMessageVoid(self->eventReceiver, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
void HCButtonPrint(HCButtonRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->base.base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCStringRef HCButtonTitleRetained(HCButtonRef self) {
    id titleNSString = HCObjcSendIdMessageVoid(self->base.view, sel_getUid("title"));
    return HCStringCreateWithNSString(titleNSString);
}

void HCButtonSetTitle(HCButtonRef self, HCStringRef title) {
    id titleNSString = NSStringAllocInitWithHCString(title);
    HCObjcSendVoidMessageId(self->base.view, sel_getUid("setTitle:"), titleNSString);
    HCObjcSendVoidMessageVoid(titleNSString, sel_getUid("release"));
}

HCButtonClickFunction HCButtonClickCallback(HCButtonRef self) {
    return self->clickCallback;
}

void HCButtonSetClickCallback(HCButtonRef self, HCButtonClickFunction callback, void* context) {
    self->clickCallback = callback;
    self->clickContext = context;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
void HCButtonClickEvent(id eventReceiver, SEL _cmd, id sender) {
    ptrdiff_t offset = ivar_getOffset(class_getInstanceVariable(g_EventReceiverClass, "hcButton"));
    HCButtonRef self = *(HCButtonRef*)((uint8_t*)eventReceiver + offset);
    if (self->clickCallback != NULL) {
        self->clickCallback(self->clickContext, self);
    }
}
