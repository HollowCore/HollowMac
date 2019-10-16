//
//  HCMac.h
//  HollowMac
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCMac_h
#define HCMac_h

#include <objc/message.h>
#include <objc/runtime.h>

typedef struct NSPoint {
    double x;
    double y;
} NSPoint;

typedef struct NSSize {
    double width;
    double height;
} NSSize;

typedef struct NSRect {
    NSPoint origin;
    NSSize size;
} NSRect;

enum {
    NSBorderlessWindowMask        = 0,
    NSTitledWindowMask            = 1 << 0,
    NSClosableWindowMask          = 1 << 1,
    NSMiniaturizableWindowMask    = 1 << 2,
    NSResizableWindowMask         = 1 << 3,
};

typedef void (*HCObjCVoidMessageVoid)(id, SEL);
typedef void (*HCObjCVoidMessageId)(id, SEL, id);
typedef id (*HCObjCIdMessageVoid)(id, SEL);
typedef id (*HCObjCIdMessageId)(id, SEL, id);
typedef id(*HCObjCIdMessageNSRect)(id, SEL, NSRect);
typedef id(*HCObjCIdMessageNSRectIntIntBool)(id, SEL, NSRect, int, int, bool);

static HCObjCVoidMessageVoid HCObjCSendVoidMessageVoid = (HCObjCVoidMessageVoid)objc_msgSend;
static HCObjCVoidMessageId HCObjCSendVoidMessageId = (HCObjCVoidMessageId)objc_msgSend;
static HCObjCIdMessageVoid HCObjCSendIdMessageVoid = (HCObjCIdMessageVoid)objc_msgSend;
static HCObjCIdMessageId HCObjCSendIdMessageId = (HCObjCIdMessageId)objc_msgSend;
static HCObjCIdMessageNSRect HCObjCSendIdMessageNSRect = (HCObjCIdMessageNSRect)objc_msgSend;
static HCObjCIdMessageNSRectIntIntBool HCObjCSendIdMessageNSRectIntIntBool = (HCObjCIdMessageNSRectIntIntBool)objc_msgSend;

extern void NSRectFill(NSRect aRect);

#endif /* HCMac_h */
