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
Class g_ButtonEventReceiverClass = NULL;
void HCButtonClickEvent(id eventReceiver, SEL _cmd, id sender);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCButtonRef HCButtonCreate() {
    // Create button
    id nsButton = HCObjcSendIdMessageCGRect(HCObjcSendIdMessageVoid((id)objc_getClass("NSButton"), sel_getUid("alloc")), sel_getUid("initWithFrame:"), (CGRect){ 0, 0, 0, 0 });
    
    return HCButtonCreateWithNSButton(nsButton);
}

HCButtonRef HCButtonCreateWithNSButton(id nsButton) {
    HCButtonRef self = calloc(sizeof(HCButton), 1);
    HCButtonInit(self, nsButton);
    return self;
}

void HCButtonInit(void* memory, id nsButton) {
    // Register event receiver NSObject subclass
    // TODO: Multi-thread safe
    if (g_ButtonEventReceiverClass == NULL) {
        g_ButtonEventReceiverClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "ButtonEventReceiver", 0);
        class_addMethod(g_ButtonEventReceiverClass, sel_getUid("click:"), (IMP)HCButtonClickEvent, "v@:@");
        class_addIvar(g_ButtonEventReceiverClass, "hcButton", sizeof(uint64_t), log2(sizeof(uint64_t)), "Q");
        objc_registerClassPair(g_ButtonEventReceiverClass);
    }
    
    // Create event receiver
    id eventReceiver = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)g_ButtonEventReceiverClass, sel_getUid("alloc")), sel_getUid("init"));
    
    // Register button click callback
    HCObjcSendVoidMessageId(nsButton, sel_getUid("setTarget:"), eventReceiver);
    HCObjcSendVoidMessageId(nsButton, sel_getUid("setAction:"), (id)sel_getUid("click:"));
    
    // Initialize button object
    HCViewInit(memory, nsButton);
    HCButtonRef self = memory;
    self->base.base.type = HCButtonType;
    self->eventReceiver = eventReceiver;
    
    // Put self into event receiver for callbacks
    ptrdiff_t offset = ivar_getOffset(class_getInstanceVariable(g_ButtonEventReceiverClass, "hcButton"));
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
    id titleNSString = HCObjcSendIdMessageVoid(self->base.nsView, sel_getUid("title"));
    return HCStringCreateWithNSString(titleNSString);
}

void HCButtonSetTitle(HCButtonRef self, HCStringRef title) {
    id titleNSString = NSStringAllocInitWithHCString(title);
    HCObjcSendVoidMessageId(self->base.nsView, sel_getUid("setTitle:"), titleNSString);
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
    ptrdiff_t offset = ivar_getOffset(class_getInstanceVariable(g_ButtonEventReceiverClass, "hcButton"));
    HCButtonRef self = *(HCButtonRef*)((uint8_t*)eventReceiver + offset);
    if (self->clickCallback != NULL) {
        self->clickCallback(self->clickContext, self);
    }
}
