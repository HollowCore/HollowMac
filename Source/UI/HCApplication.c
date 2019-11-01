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

BOOL HCApplicationDelegateApplicationDidFinishLaunching(HCApplicationDelegate* self, SEL _cmd, id notification);
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
    id mainMenu = HCObjcSendIdMessageVoid(NSApp, sel_getUid("mainMenu"));
    return mainMenu == NULL ? 0 : HCObjcSendNSIntegerMessageVoid(mainMenu, sel_getUid("numberOfItems"));
}

HCMenuRef HCApplicationMenuAtIndexRetained(HCApplicationRef self, HCInteger index) {
    id mainMenu = HCObjcSendIdMessageVoid(NSApp, sel_getUid("mainMenu"));
    if (mainMenu == NULL) {
        return NULL;
    }
    id nsMenuItem = HCObjcSendIdMessageNSInteger(mainMenu, sel_getUid("itemAtIndex:"), index);
    return HCMenuCreateWithNSMenuItem(nsMenuItem);
}

void HCApplicationAddMenu(HCApplicationRef self, HCMenuRef menu) {
    id mainMenu = HCObjcSendIdMessageVoid(NSApp, sel_getUid("mainMenu"));
    if (mainMenu == NULL) {
        mainMenu = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSMenu"), sel_getUid("alloc")), sel_getUid("init"));
        HCObjcSendVoidMessageId(NSApp, sel_getUid("setMainMenu:"), mainMenu);
    }
    HCObjcSendVoidMessageId(mainMenu, sel_getUid("addItem:"), menu->nsMenuItem);
}

void HCApplicationRemoveMenu(HCApplicationRef self, HCInteger index) {
    id mainMenu = HCObjcSendIdMessageVoid(NSApp, sel_getUid("mainMenu"));
    if (mainMenu == NULL) {
        return;
    }
    HCObjcSendVoidMessageNSInteger(mainMenu, sel_getUid("removeItemAtIndex:"), index);
    if (HCObjcSendNSIntegerMessageVoid(mainMenu, sel_getUid("numberOfItems")) == 0) {
        HCObjcSendVoidMessageId(NSApp, sel_getUid("setMainMenu:"), NULL);
    }
}

//void HCApplicationApplyTestMainMenu(HCApplicationRef self) {
//    id quitItem = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSMenuItem"), sel_getUid("alloc")), sel_getUid("init"));
//    HCObjcSendVoidMessageId(quitItem, sel_getUid("setTitle:"), HCObjcSendIdMessageId((id)objc_getClass("NSString"), sel_getUid("stringWithUTF8String:"), (id)"Quit"));
//    HCObjcSendVoidMessageId(quitItem, sel_getUid("setTarget:"), NSApp);
//    HCObjcSendVoidMessageId(quitItem, sel_getUid("setAction:"), (id)sel_getUid("terminate:"));
//
//    id applicationMenu = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSMenu"), sel_getUid("alloc")), sel_getUid("init"));
//    HCObjcSendVoidMessageId(applicationMenu, sel_getUid("addItem:"), quitItem);
//
//    id applicationMenuItem = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSMenuItem"), sel_getUid("alloc")), sel_getUid("init"));
//    HCObjcSendVoidMessageId(applicationMenuItem, sel_getUid("setSubmenu:"), applicationMenu);
//
//    id mainMenu = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)objc_getClass("NSMenu"), sel_getUid("alloc")), sel_getUid("init"));
//    HCObjcSendVoidMessageId(mainMenu, sel_getUid("addItem:"), applicationMenuItem);
//
//    HCObjcSendVoidMessageId(NSApp, sel_getUid("setMainMenu:"), mainMenu);
//}

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
    HCObjcSendVoidMessageId(NSApp, sel_getUid("terminate:"), NSApp);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
BOOL HCApplicationDelegateApplicationDidFinishLaunching(HCApplicationDelegate* applicationDelegate, SEL _cmd, id notification) {
    HCApplicationRef self = applicationDelegate->application;
    
    // Call ready callback
    if (self->readyCallback != NULL) {
        self->readyCallback(self->readyCallbackContext, self);
    }
    
    return true;
}

BOOL HCApplicationDelegateApplicationShouldTerminateAfterLastWindowClosed(HCApplicationDelegate* applicationDelegate) {
    return true;
}
