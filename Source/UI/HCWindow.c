//
//  HCWindow.c
//  Test
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCWindow_Internal.h"
#include "HCView_Internal.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCWindowTypeDataInstance = {
    .base = {
        .ancestor = (HCType)&HCObjectTypeDataInstance,
        .name = "HCWindow",
    },
    .isEqual = (void*)HCWindowIsEqual,
    .hashValue = (void*)HCWindowHashValue,
    .print = (void*)HCWindowPrint,
    .destroy = (void*)HCWindowDestroy,
};
HCType HCWindowType = (HCType)&HCWindowTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
// TODO: Put these into type struct
Class g_WindowEventReceiverClass = NULL;
void HCWindowResizeNotification(id eventReceiver, SEL cmd, id notification);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowRef HCWindowCreate() {
    HCWindowRef self = calloc(sizeof(HCWindow), 1);
    HCWindowInit(self);
    return self;
}

void HCWindowInit(void* memory) {
    // Register event receiver NSObject subclass
    // TODO: Multi-thread safe
    if (g_WindowEventReceiverClass == NULL) {
        g_WindowEventReceiverClass = objc_allocateClassPair((Class)objc_getClass("NSObject"), "WindowEventReceiver", 0);
        class_addMethod(g_WindowEventReceiverClass, sel_getUid("resize:"), (IMP)HCWindowResizeNotification, "v@:@");
        class_addIvar(g_WindowEventReceiverClass, "hcWindow", sizeof(uint64_t), log2(sizeof(uint64_t)), "Q");
        objc_registerClassPair(g_WindowEventReceiverClass);
    }
    
    // Create event receiver
    id eventReceiver = HCObjcSendIdMessageVoid(HCObjcSendIdMessageVoid((id)g_WindowEventReceiverClass, sel_getUid("alloc")), sel_getUid("init"));
    
    // Find default size
    id mainScreen = HCObjcSendIdMessageVoid((id)objc_getClass("NSScreen"), sel_getUid("mainScreen"));
    CGRect mainScreenFrame = HCObjcSendCGRectMessageVoid(mainScreen, sel_getUid("frame"));
    
    // Create window
    int styleMask = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask;
    CGRect contentRect = HCObjcSendCGRectMessageCGRectInt((id)objc_getClass("NSWindow"), sel_getUid("contentRectForFrameRect:styleMask:"), mainScreenFrame, styleMask);
    id window = HCObjcSendIdMessageVoid((id)objc_getClass("NSWindow"), sel_getUid("alloc"));
    window = HCObjcSendIdMessageCGRectIntIntBool(
        window,
        sel_getUid("initWithContentRect:styleMask:backing:defer:"),
        contentRect,
        styleMask,
        0,
        false
    );
    
    // Register resize notification
    id notificationCenter = HCObjcSendIdMessageVoid((id)objc_getClass("NSNotificationCenter"), sel_getUid("defaultCenter"));
    id nsWindowDidResizeNotificationName = NSStringAllocInitWithCString("NSWindowDidResizeNotification");
    HCObjcSendVoidMessageIdSELIdId(notificationCenter, sel_getUid("addObserver:selector:name:object:"), eventReceiver, sel_getUid("resize:"), nsWindowDidResizeNotificationName, NULL);
    HCObjcSendRelease(nsWindowDidResizeNotificationName);
    
    // Initialize window object
    HCObjectInit(memory);
    HCWindowRef self = memory;
    self->base.type = HCWindowType;
    self->nsWindow = window;
    self->eventReceiver = eventReceiver;
    
    // Create window content view
    HCViewRef contentView = HCViewCreate();
    self->contentView = contentView;
    HCViewSetFrame(contentView, HCWindowContentRectangle(self));
    HCObjcSendVoidMessageId(window, sel_getUid("setContentView:"), contentView->nsView);

    // Put self into event receiver for callbacks
    ptrdiff_t offset = ivar_getOffset(class_getInstanceVariable(g_WindowEventReceiverClass, "hcWindow"));
    *(HCWindowRef*)((uint8_t*)eventReceiver + offset) = self;
}

void HCWindowDestroy(HCWindowRef self) {
    // Unregister resize notification
    id notificationCenter = HCObjcSendIdMessageVoid((id)objc_getClass("NSNotificationCenter"), sel_getUid("defaultCenter"));
    id nsWindowDidResizeNotificationName = NSStringAllocInitWithCString("NSWindowDidResizeNotification");
    HCObjcSendVoidMessageIdIdId(notificationCenter, sel_getUid("removeObserver:name:object:"), self->eventReceiver, nsWindowDidResizeNotificationName, NULL);
    HCObjcSendRelease(nsWindowDidResizeNotificationName);
    
    HCObjcSendRelease(self->eventReceiver);
    HCRelease(self->contentView);
    HCObjcSendRelease(self->nsWindow);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCWindowIsEqual(HCWindowRef self, HCWindowRef other) {
    return
        HCIntegerIsEqual((HCInteger)self->nsWindow, (HCInteger)other->nsWindow) &&
        HCIsEqual(self->contentView, other->contentView);
}

HCInteger HCWindowHashValue(HCWindowRef self) {
    return
        HCIntegerHashValue((HCInteger)self->nsWindow) ^
        HCHashValue(self->contentView);
}

void HCWindowPrint(HCWindowRef self, FILE* stream) {
    fprintf(stream, "<%s@%p>", self->base.type->name, self);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowCoordinateLocation HCWindowOriginLocation(HCWindowRef self) {
    (void)self; // Unused
    return HCWindowCoordinateLocationLowerLeft;
}

HCPoint HCWindowCenter(HCWindowRef self) {
    HCRectangle frame = HCWindowFrame(self);
    return HCPointMake(HCRectangleMidX(frame), HCRectangleMidY(frame));
}

void HCWindowSetCenter(HCWindowRef self, HCPoint center) {
    HCRectangle frame = HCWindowFrame(self);
    HCReal dx = center.x - HCRectangleMidX(frame);
    HCReal dy = center.y - HCRectangleMidY(frame);
    frame = HCRectangleOffset(frame, dx, dy);
    HCWindowSetFrame(self, frame);
}

HCPoint HCWindowOrigin(HCWindowRef self) {
    HCRectangle frame = HCWindowFrame(self);
    return frame.origin;
}

void HCWindowSetOrigin(HCWindowRef self, HCPoint origin) {
    HCRectangle frame = HCWindowFrame(self);
    frame.origin = origin;
    HCWindowSetFrame(self, frame);
}

HCSize HCWindowSize(HCWindowRef self) {
    HCRectangle frame = HCWindowFrame(self);
    return frame.size;
}

void HCWindowSetSize(HCWindowRef self, HCSize size) {
    HCRectangle frame = HCWindowFrame(self);
    frame.size = size;
    HCWindowSetFrame(self, frame);
}

HCRectangle HCWindowFrame(HCWindowRef self) {
    CGRect frame = HCObjcSendCGRectMessageVoid(self->nsWindow, sel_getUid("frame"));
    return HCRectangleMakeWithCGRect(frame);
}

void HCWindowSetFrame(HCWindowRef self, HCRectangle frame) {
    HCObjcSendVoidMessageCGRectBool(self->nsWindow, sel_getUid("setFrame:display:"), CGRectMakeWithHCRectangle(frame), true);
}

HCRectangle HCWindowContentRectangle(HCWindowRef self) {
    CGRect screenRelativeContentRect = HCObjcSendCGRectMessageCGRect(self->nsWindow, sel_getUid("contentRectForFrameRect:"), CGRectMakeWithHCRectangle(HCWindowFrame(self)));
    CGRect contentRect = HCObjcSendCGRectMessageCGRect(self->nsWindow, sel_getUid("convertRectFromScreen:"), screenRelativeContentRect);
    return HCRectangleMakeWithCGRect(contentRect);
}

HCViewRef HCWindowContentView(HCWindowRef self) {
    return self->contentView;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Events
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowResizeFunction HCWindowResizeCallback(HCWindowRef self) {
    return self->resizeCallback;
}

void HCWindowSetResizeCallback(HCWindowRef self, HCWindowResizeFunction callback, void* context) {
    self->resizeCallback = callback;
    self->resizeContext = context;
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowDisplay(HCWindowRef self) {
    HCObjcSendVoidMessageVoid(self->nsWindow, sel_getUid("becomeFirstResponder"));
    HCObjcSendVoidMessageId(self->nsWindow, sel_getUid("makeKeyAndOrderFront:"), NULL);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowResizeNotification(id eventReceiver, SEL cmd, id notification) {
    (void)cmd; (void)notification; // Unused
    ptrdiff_t offset = ivar_getOffset(class_getInstanceVariable(g_WindowEventReceiverClass, "hcWindow"));
    HCWindowRef self = *(HCWindowRef*)((uint8_t*)eventReceiver + offset);
    
    // Resize content view
    HCRectangle contentRectangle = HCWindowContentRectangle(self);
    HCViewSetFrame(self->contentView, HCRectangleMake(HCPointZero, contentRectangle.size));
    
    // Call registered callback
    if (self->resizeCallback != NULL) {
        self->resizeCallback(self->resizeContext, self, contentRectangle.size);
    }
}
