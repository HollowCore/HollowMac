//
//  HCMenu.c
//  HollowMac
//
//  Created by Matt Stoker on 10/31/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCMenu_Internal.h"
#include "HCView_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCMenuTypeDataInstance = {
    .base = {
        .ancestor = (HCType)&HCObjectTypeDataInstance,
        .name = "HCMenu",
    },
    .isEqual = (void*)HCMenuIsEqual,
    .hashValue = (void*)HCMenuHashValue,
    .print = (void*)HCMenuPrint,
    .destroy = (void*)HCMenuDestroy,
};
HCType HCMenuType = (HCType)&HCMenuTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------

// TODO: Put these into type struct
Class g_MenuEventReceiverClass = NULL;
void HCMenuClickEvent(id eventReceiver, SEL cmd, id sender);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCMenuRef HCMenuCreate() {
    // Create NSMenuItem
    id nsMenuItem = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSMenuItem"), sel_getUid("alloc")), sel_getUid("init"));
    
    // Create menu
    HCMenuRef menu = HCMenuCreateWithNSMenuItem(nsMenuItem);
    
    // Cleanup
    HCObjcSendVoidMessageVoid(nsMenuItem, sel_getUid("release"));
    
    return menu;
}

HCMenuRef HCMenuCreateWithNSMenuItem(id nsMenuItem) {
    HCMenuRef self = calloc(sizeof(HCMenu), 1);
    HCMenuInit(self, nsMenuItem);
    return self;
}

void HCMenuInit(void* memory, id nsMenuItem) {
    // Register event receiver NSObject subclass
    // TODO: Multi-thread safe
    if (g_MenuEventReceiverClass == NULL) {
        g_MenuEventReceiverClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "MenuEventReceiver", 0);
        class_addMethod(g_MenuEventReceiverClass, sel_getUid("click:"), (IMP)HCMenuClickEvent, "v@:@");
        class_addIvar(g_MenuEventReceiverClass, "hcMenu", sizeof(uint64_t), log2(sizeof(uint64_t)), "Q");
        objc_registerClassPair(g_MenuEventReceiverClass);
    }
    
    // Create event receiver
    id eventReceiver = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)g_MenuEventReceiverClass, sel_getUid("alloc")), sel_getUid("init"));
    
    // Attach to menu
    HCObjcSendVoidMessageId(nsMenuItem, sel_getUid("setTarget:"), eventReceiver);
    HCObjcSendVoidMessageId(nsMenuItem, sel_getUid("setAction:"), (id)sel_getUid("click:"));
    
    // Initialize menu object
    HCObjectInit(memory);
    HCMenuRef self = memory;
    self->base.type = HCMenuType;
    self->nsMenuItem = HCObjcSendIdMessageVoid(nsMenuItem, sel_getUid("retain"));
    self->eventReceiver = eventReceiver;
    
    // Put self into event receiver for callbacks
    ptrdiff_t offset = ivar_getOffset(class_getInstanceVariable(g_MenuEventReceiverClass, "hcMenu"));
    *(HCMenuRef*)((uint8_t*)eventReceiver + offset) = self;
}

void HCMenuDestroy(HCMenuRef self) {
    HCObjcSendVoidMessageVoid(self->eventReceiver, sel_getUid("release"));
    HCObjcSendVoidMessageVoid(self->nsMenuItem, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMenuIsEqual(HCMenuRef self, HCMenuRef other) {
    return
        HCIntegerIsEqual((HCInteger)self->nsMenuItem, (HCInteger)other->nsMenuItem);
}

HCInteger HCMenuHashValue(HCMenuRef self) {
    return
        HCIntegerHashValue((HCInteger)self->nsMenuItem);
}

void HCMenuPrint(HCMenuRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCStringRef HCMenuTitleRetained(HCMenuRef self) {
    id titleNSString = HCObjcSendIdMessageVoid(self->nsMenuItem, sel_getUid("title"));
    return HCStringCreateWithNSString(titleNSString);
}

void HCMenuSetTitle(HCMenuRef self, HCStringRef title) {
    id titleNSString = NSStringAllocInitWithHCString(title);
    HCObjcSendVoidMessageId(self->nsMenuItem, sel_getUid("setTitle:"), titleNSString);
    HCObjcSendVoidMessageVoid(titleNSString, sel_getUid("release"));
}

HCMenuClickFunction HCMenuClickCallback(HCMenuRef self) {
    return self->clickCallback;
}

void HCMenuSetClickCallback(HCMenuRef self, HCMenuClickFunction callback, void* context) {
    self->clickCallback = callback;
    self->clickContext = context;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Related Menus
//----------------------------------------------------------------------------------------------------------------------------------
HCMenuRef HCMenuParentMenuRetained(HCMenuRef self) {
    id parentItem = HCObjcSendIdMessageVoid(self->nsMenuItem, sel_getUid("parentItem"));
    return HCMenuCreateWithNSMenuItem(parentItem);
}

HCInteger HCMenuChildMenuCount(HCMenuRef self) {
    id submenu = HCObjcSendIdMessageVoid(self->nsMenuItem, sel_getUid("submenu"));
    return submenu == NULL ? 0 : HCObjcSendNSIntegerMessageVoid(submenu, sel_getUid("numberOfItems"));
}

HCMenuRef HCMenuChildMenuAtIndexRetained(HCMenuRef self, HCInteger index) {
    id submenu = HCObjcSendIdMessageVoid(self->nsMenuItem, sel_getUid("submenu"));
    if (submenu == NULL) {
        return NULL;
    }
    id nsMenuItem = HCObjcSendIdMessageNSInteger(submenu, sel_getUid("itemAtIndex:"), index);
    return nsMenuItem == NULL ? NULL : HCMenuCreateWithNSMenuItem(nsMenuItem);
}

void HCMenuAddChildMenu(HCMenuRef self, HCMenuRef child) {
    id submenu = HCObjcSendIdMessageVoid(self->nsMenuItem, sel_getUid("submenu"));
    if (submenu == NULL) {
        submenu = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSMenu"), sel_getUid("alloc")), sel_getUid("init"));
        HCObjcSendVoidMessageId(self->nsMenuItem, sel_getUid("setSubmenu:"), submenu);
        HCObjcSendVoidMessageVoid(submenu, sel_getUid("release"));
    }
    HCObjcSendVoidMessageId(submenu, sel_getUid("addItem:"), child->nsMenuItem);
}

void HCMenuRemoveChildMenu(HCMenuRef self, HCInteger index) {
    id submenu = HCObjcSendIdMessageVoid(self->nsMenuItem, sel_getUid("submenu"));
    if (submenu == NULL) {
        return;
    }
    HCObjcSendVoidMessageNSInteger(submenu, sel_getUid("removeItemAtIndex:"), index);
    if (HCObjcSendNSIntegerMessageVoid(submenu, sel_getUid("numberOfItems")) == 0) {
        HCObjcSendVoidMessageId(self->nsMenuItem, sel_getUid("setSubmenu:"), NULL);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
void HCMenuClickEvent(id eventReceiver, SEL cmd, id sender) {
    (void)cmd; // Unused
    (void)sender; // Unused
    ptrdiff_t offset = ivar_getOffset(class_getInstanceVariable(g_MenuEventReceiverClass, "hcMenu"));
    HCMenuRef self = *(HCMenuRef*)((uint8_t*)eventReceiver + offset);
    if (self->clickCallback != NULL) {
        self->clickCallback(self->clickContext, self);
    }
}
