//
//  HCApplication.c
//  HollowMac
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCApplication_Internal.h"
#include "../../HollowCore/Source/Core/HCCore.h"
#include <math.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCApplicationTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
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

BOOL HCApplicationDelegateDidFinishLaunching(HCApplicationDelegate* self, SEL _cmd, id notification);

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
        class_addMethod(g_ApplicationDelegateClass, sel_getUid("applicationDidFinishLaunching:"), (IMP)HCApplicationDelegateDidFinishLaunching, "i@:@");
        objc_registerClassPair(g_ApplicationDelegateClass);
    }
    
    // Initialize shared macOS application object
    if (NSApp == NULL) {
        HCObjCSendVoidMessageVoid((id)objc_getClass("NSApplication"), sel_getUid("sharedApplication"));
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

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCApplicationRun(HCApplicationRef self, HCApplicationReadyCallback readyCallback, void* readyCallbackContext) {
    self->readyCallback = readyCallback;
    self->readyCallbackContext = readyCallbackContext;
    
    id applicationDelegateObject = HCObjCSendIdMessageVoid((id)objc_getClass("AppDelegate"), sel_getUid("alloc"));
    applicationDelegateObject = HCObjCSendIdMessageVoid(applicationDelegateObject, sel_getUid("init"));
    ((HCApplicationDelegate*)applicationDelegateObject)->application = HCRetain(self);
    HCObjCSendVoidMessageId(NSApp, sel_getUid("setDelegate:"), applicationDelegateObject);
    HCObjCSendVoidMessageVoid(NSApp, sel_getUid("run"));
}

void HCApplicationTerminate(HCApplicationRef self) {
    HCObjCSendVoidMessageId(NSApp, sel_getUid("terminate:"), NSApp);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
BOOL HCApplicationDelegateDidFinishLaunching(HCApplicationDelegate* applicationDelegate, SEL _cmd, id notification) {
    HCApplicationRef self = applicationDelegate->application;
    
    // Call ready callback
    if (self->readyCallback != NULL) {
        self->readyCallback(self->readyCallbackContext, self);
    }
    
    return true;
}
