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
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct ApplicationDelegate {
    Class isa;
    id window;
} ApplicationDelegate;

BOOL ApplicationDelegateDidFinishLaunching(ApplicationDelegate* self, SEL _cmd, id notification);
void ViewDrawRect(id self, SEL _cmd, NSRect rect);

// TODO: Put these into type struct
Class g_ApplicationDelegateClass = NULL;
Class g_ViewClass = NULL;

extern id NSApp;
extern void NSRectFill(NSRect aRect);

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
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCApplicationRef HCApplicationCreate() {
    HCApplicationRef self = calloc(sizeof(HCApplication), 1);
    HCApplicationInit(self);
    return self;
}

void HCApplicationInit(void* memory) {
    HCObjectInit(memory);
    HCApplicationRef self = memory;
    self->base.type = HCApplicationType;
    
    // TODO: Multithread safe
    if (g_ApplicationDelegateClass == NULL) {
        // Register ApplicationDelegate class
        g_ApplicationDelegateClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "AppDelegate", 0);
        class_addMethod(g_ApplicationDelegateClass, sel_getUid("applicationDidFinishLaunching:"), (IMP)ApplicationDelegateDidFinishLaunching, "i@:@");
        objc_registerClassPair(g_ApplicationDelegateClass);
        
        // Register View class
        g_ViewClass = objc_allocateClassPair((Class)objc_getClass("NSView"), "View", 0);
        class_addMethod(g_ViewClass, sel_getUid("drawRect:"), (IMP) ViewDrawRect, "v@:");
        objc_registerClassPair(g_ViewClass);
        
        HCObjCSendVoidMessageVoid((id)objc_getClass("NSApplication"), sel_getUid("sharedApplication"));
        
        if (NSApp == NULL) {
            fprintf(stderr,"Failed to initialized NSApplication...  terminating...\n");
            return;
        }
        
        id applicationDelegateObject = HCObjCSendIdMessageVoid((id)objc_getClass("AppDelegate"), sel_getUid("alloc"));
        applicationDelegateObject = HCObjCSendIdMessageVoid(applicationDelegateObject, sel_getUid("init"));
        
        HCObjCSendVoidMessageId(NSApp, sel_getUid("setDelegate:"), applicationDelegateObject);
        HCObjCSendVoidMessageVoid(NSApp, sel_getUid("run"));
    }
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
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
BOOL ApplicationDelegateDidFinishLaunching(ApplicationDelegate* self, SEL _cmd, id notification) {
    self->window = HCObjCSendIdMessageVoid((id)objc_getClass("NSWindow"), sel_getUid("alloc"));

    self->window = HCObjCSendIdMessageNSRectIntIntBool(self->window, sel_getUid("initWithContentRect:styleMask:backing:defer:"), (NSRect){ 0, 0, 720, 480 }, (NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask), 0, false);
    id view = HCObjCSendIdMessageNSRect(HCObjCSendIdMessageVoid((id)objc_getClass("View"), sel_getUid("alloc")), sel_getUid("initWithFrame:"), (NSRect){ 0, 0, 200, 100 });

    HCObjCSendVoidMessageId(self->window, sel_getUid("setContentView:"), view);
    HCObjCSendVoidMessageVoid(self->window, sel_getUid("becomeFirstResponder"));
    HCObjCSendVoidMessageId(self->window, sel_getUid("makeKeyAndOrderFront:"), (id)self);
    return true;
}

void ViewDrawRect(id self, SEL _cmd, NSRect rect) {
    id redColor = HCObjCSendIdMessageVoid((id)objc_getClass("NSColor"), sel_getUid("redColor"));
    
    NSRect rect1 = (NSRect){ 21, 21, 210, 210 };
    HCObjCSendVoidMessageVoid(redColor, sel_getUid("set"));
    NSRectFill(rect1);
}
