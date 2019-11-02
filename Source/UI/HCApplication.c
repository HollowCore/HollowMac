//
//  HCApplication.c
//  HollowMac
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCApplication_Internal.h"
#include "HCMenu_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCApplicationTypeDataInstance = {
    .base = {
        .ancestor = (HCType)&HCObjectTypeDataInstance,
        .name = "HCApplication",
    },
    .isEqual = (void*)HCApplicationIsEqual,
    .hashValue = (void*)HCApplicationHashValue,
    .print = (void*)HCApplicationPrint,
    .destroy = (void*)HCApplicationDestroy,
};
HCType HCApplicationType = (HCType)&HCApplicationTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCApplicationDelegate {
    Class isa;
    HCApplicationRef application;
} HCApplicationDelegate;

BOOL HCApplicationDelegateApplicationDidFinishLaunching(HCApplicationDelegate* self, SEL cmd, id notification);
BOOL HCApplicationDelegateApplicationShouldTerminateAfterLastWindowClosed(HCApplicationDelegate* applicationDelegate);

// TODO: Put these into type struct
Class g_ApplicationDelegateClass = NULL;

extern id NSApp;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCApplicationRef HCApplicationCreate() {
    HCApplicationRef self = calloc(sizeof(HCApplication), 1);
    HCApplicationInit(self);
    return self;
}

void HCApplicationInit(void* memory) {
    // Register application delegate class
    // TODO: Multithread safe
    if (g_ApplicationDelegateClass == NULL) {
        g_ApplicationDelegateClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "AppDelegate", 0);
        class_addMethod(g_ApplicationDelegateClass, sel_getUid("applicationDidFinishLaunching:"), (IMP)HCApplicationDelegateApplicationDidFinishLaunching, "i@:@");
        class_addMethod(g_ApplicationDelegateClass, sel_getUid("applicationShouldTerminateAfterLastWindowClosed:"), (IMP)HCApplicationDelegateApplicationShouldTerminateAfterLastWindowClosed, "i@:");
        objc_registerClassPair(g_ApplicationDelegateClass);
    }
    
    // Initialize shared macOS application object
    if (NSApp == NULL) {
        HCObjcSendVoidMessageVoid((id)objc_getClass("NSApplication"), sel_getUid("sharedApplication"));
    }
    if (NSApp == NULL) {
        fprintf(stderr, "Failed to initialize NSApplication. Failing...\n");
        return;
    }
    
    // Initialize application object
    HCObjectInit(memory);
    HCApplicationRef self = memory;
    self->base.type = HCApplicationType;
}

void HCApplicationDestroy(HCApplicationRef self) {
    (void)self; // Unused
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCApplicationIsEqual(HCApplicationRef self, HCApplicationRef other) {
    return self == other;
}

HCInteger HCApplicationHashValue(HCApplicationRef self) {
    return (HCInteger)self;
}

void HCApplicationPrint(HCApplicationRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCApplicationMenuCount(HCApplicationRef self) {
    (void)self; // Unused
    id mainMenu = HCObjcSendIdMessageVoid(NSApp, sel_getUid("mainMenu"));
    return mainMenu == NULL ? 0 : HCObjcSendNSIntegerMessageVoid(mainMenu, sel_getUid("numberOfItems"));
}

HCMenuRef HCApplicationMenuAtIndexRetained(HCApplicationRef self, HCInteger index) {
    (void)self; // Unused
    id mainMenu = HCObjcSendIdMessageVoid(NSApp, sel_getUid("mainMenu"));
    if (mainMenu == NULL) {
        return NULL;
    }
    id nsMenuItem = HCObjcSendIdMessageNSInteger(mainMenu, sel_getUid("itemAtIndex:"), index);
    return HCMenuCreateWithNSMenuItem(nsMenuItem);
}

void HCApplicationAddMenu(HCApplicationRef self, HCMenuRef menu) {
    (void)self; // Unused
    id mainMenu = HCObjcSendIdMessageVoid(NSApp, sel_getUid("mainMenu"));
    if (mainMenu == NULL) {
        mainMenu = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSMenu"), sel_getUid("alloc")), sel_getUid("init"));
        HCObjcSendVoidMessageId(NSApp, sel_getUid("setMainMenu:"), mainMenu);
    }
    HCObjcSendVoidMessageId(mainMenu, sel_getUid("addItem:"), menu->nsMenuItem);
}

void HCApplicationRemoveMenu(HCApplicationRef self, HCInteger index) {
    (void)self; // Unused
    id mainMenu = HCObjcSendIdMessageVoid(NSApp, sel_getUid("mainMenu"));
    if (mainMenu == NULL) {
        return;
    }
    HCObjcSendVoidMessageNSInteger(mainMenu, sel_getUid("removeItemAtIndex:"), index);
    if (HCObjcSendNSIntegerMessageVoid(mainMenu, sel_getUid("numberOfItems")) == 0) {
        HCObjcSendVoidMessageId(NSApp, sel_getUid("setMainMenu:"), NULL);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCApplicationRun(HCApplicationRef self, HCApplicationReadyCallback readyCallback, void* readyCallbackContext) {
    self->readyCallback = readyCallback;
    self->readyCallbackContext = readyCallbackContext;
    
    id applicationDelegateObject = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("AppDelegate"), sel_getUid("alloc")), sel_getUid("init"));
    ((HCApplicationDelegate*)applicationDelegateObject)->application = HCRetain(self);
    HCObjcSendVoidMessageId(NSApp, sel_getUid("setDelegate:"), applicationDelegateObject);
    HCObjcSendVoidMessageVoid(NSApp, sel_getUid("run"));
}

void HCApplicationTerminate(HCApplicationRef self) {
    (void)self; // Unused
    HCObjcSendVoidMessageId(NSApp, sel_getUid("terminate:"), NSApp);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
BOOL HCApplicationDelegateApplicationDidFinishLaunching(HCApplicationDelegate* applicationDelegate, SEL cmd, id notification) {
    (void)cmd; // Unused
    (void)notification; // Unused
    HCApplicationRef self = applicationDelegate->application;
    
    // Call ready callback
    if (self->readyCallback != NULL) {
        self->readyCallback(self->readyCallbackContext, self);
    }
    
    return true;
}

BOOL HCApplicationDelegateApplicationShouldTerminateAfterLastWindowClosed(HCApplicationDelegate* applicationDelegate) {
    (void)applicationDelegate; // Unused
    return true;
}
