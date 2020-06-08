//
//  HCObjC.h
//  HollowMac
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCObjC_h
#define HCObjC_h

#include "../../HollowCore/Source/HollowCore.h"
#include <objc/message.h>
#include <objc/runtime.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Redefined Types
//----------------------------------------------------------------------------------------------------------------------------------
// TODO: Assumes 64-bit architecture
typedef int64_t NSInteger;

// TODO: Assumes 64-bit architecture
typedef double CGFloat;

typedef struct CGPoint {
    CGFloat x;
    CGFloat y;
} CGPoint;

typedef struct CGSize {
    CGFloat width;
    CGFloat height;
} CGSize;

typedef struct CGRect {
    CGPoint origin;
    CGSize size;
} CGRect;

enum {
    NSBorderlessWindowMask        = 0,
    NSTitledWindowMask            = 1 << 0,
    NSClosableWindowMask          = 1 << 1,
    NSMiniaturizableWindowMask    = 1 << 2,
    NSResizableWindowMask         = 1 << 3,
};

static const int NSCalibratedRGBColorSpace = 0x8d355d78;
static const int NSDeviceRGBColorSpace = 0x8d355dd8;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Externally Defined Functions
//----------------------------------------------------------------------------------------------------------------------------------
extern void CGRectFill(CGRect aRect);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Objective-C Message Sending
//----------------------------------------------------------------------------------------------------------------------------------
typedef void (*HCObjcVoidMessageVoid)(id, SEL);
static HCObjcVoidMessageVoid HCObjcSendVoidMessageVoid = (HCObjcVoidMessageVoid)objc_msgSend;

typedef void (*HCObjcVoidMessageId)(id, SEL, id);
static HCObjcVoidMessageId HCObjcSendVoidMessageId = (HCObjcVoidMessageId)objc_msgSend;
typedef id (*HCObjcIdMessageVoid)(id, SEL);
static HCObjcIdMessageVoid HCObjcSendIdMessageVoid = (HCObjcIdMessageVoid)objc_msgSend;
typedef id (*HCObjcIdMessageId)(id, SEL, id);
static HCObjcIdMessageId HCObjcSendIdMessageId = (HCObjcIdMessageId)objc_msgSend;

typedef void (*HCObjcVoidMessageBool)(id, SEL, BOOL);
static HCObjcVoidMessageBool HCObjcSendVoidMessageBool = (HCObjcVoidMessageBool)objc_msgSend;
typedef BOOL (*HCObjcBoolMessageVoid)(id, SEL);
static HCObjcBoolMessageVoid HCObjcSendBoolMessageVoid = (HCObjcBoolMessageVoid)objc_msgSend;
typedef BOOL (*HCObjcBoolMessageBool)(id, SEL, BOOL);
static HCObjcBoolMessageBool HCObjcSendBoolMessageBool = (HCObjcBoolMessageBool)objc_msgSend;
typedef BOOL (*HCObjcBoolMessageId)(id, SEL, id);
static HCObjcBoolMessageId HCObjcSendBoolMessageId = (HCObjcBoolMessageId)objc_msgSend;
typedef id (*HCObjcIdMessageBool)(id, SEL, BOOL);
static HCObjcIdMessageBool HCObjcSendIdMessageBool = (HCObjcIdMessageBool)objc_msgSend;

typedef void (*HCObjcVoidMessageNSInteger)(id, SEL, NSInteger);
static HCObjcVoidMessageNSInteger HCObjcSendVoidMessageNSInteger = (HCObjcVoidMessageNSInteger)objc_msgSend;
typedef NSInteger (*HCObjcNSIntegerMessageVoid)(id, SEL);
static HCObjcNSIntegerMessageVoid HCObjcSendNSIntegerMessageVoid = (HCObjcNSIntegerMessageVoid)objc_msgSend;
typedef NSInteger (*HCObjcNSIntegerMessageNSInteger)(id, SEL, NSInteger);
static HCObjcNSIntegerMessageNSInteger HCObjcSendNSIntegerMessageNSInteger = (HCObjcNSIntegerMessageNSInteger)objc_msgSend;
typedef NSInteger (*HCObjcNSIntegerMessageId)(id, SEL, id);
static HCObjcNSIntegerMessageId HCObjcSendNSIntegerMessageId = (HCObjcNSIntegerMessageId)objc_msgSend;
typedef id (*HCObjcIdMessageNSInteger)(id, SEL, NSInteger);
static HCObjcIdMessageNSInteger HCObjcSendIdMessageNSInteger = (HCObjcIdMessageNSInteger)objc_msgSend;

typedef void (*HCObjcVoidMessageFloat)(id, SEL, float);
static HCObjcVoidMessageFloat HCObjcSendVoidMessageFloat = (HCObjcVoidMessageFloat)objc_msgSend;
typedef float (*HCObjcFloatMessageVoid)(id, SEL);
static HCObjcFloatMessageVoid HCObjcSendFloatMessageVoid = (HCObjcFloatMessageVoid)objc_msgSend;
typedef float (*HCObjcFloatMessageFloat)(id, SEL, float);
static HCObjcFloatMessageFloat HCObjcSendFloatMessageFloat = (HCObjcFloatMessageFloat)objc_msgSend;
typedef float (*HCObjcFloatMessageId)(id, SEL, id);
static HCObjcFloatMessageId HCObjcSendFloatMessageId = (HCObjcFloatMessageId)objc_msgSend;
typedef id (*HCObjcIdMessageFloat)(id, SEL, float);
static HCObjcIdMessageFloat HCObjcSendIdMessageFloat = (HCObjcIdMessageFloat)objc_msgSend;

typedef void (*HCObjcVoidMessageDouble)(id, SEL, double);
static HCObjcVoidMessageDouble HCObjcSendVoidMessageDouble = (HCObjcVoidMessageDouble)objc_msgSend;
typedef double (*HCObjcDoubleMessageVoid)(id, SEL);
static HCObjcDoubleMessageVoid HCObjcSendDoubleMessageVoid = (HCObjcDoubleMessageVoid)objc_msgSend;
typedef double (*HCObjcDoubleMessageDouble)(id, SEL, double);
static HCObjcDoubleMessageDouble HCObjcSendDoubleMessageDouble = (HCObjcDoubleMessageDouble)objc_msgSend;
typedef double (*HCObjcDoubleMessageId)(id, SEL, id);
static HCObjcDoubleMessageId HCObjcSendDoubleMessageId = (HCObjcDoubleMessageId)objc_msgSend;
typedef id (*HCObjcIdMessageDouble)(id, SEL, double);
static HCObjcIdMessageDouble HCObjcSendIdMessageDouble = (HCObjcIdMessageDouble)objc_msgSend;

typedef void (*HCObjcVoidMessageCGPoint)(id, SEL, CGPoint);
static HCObjcVoidMessageCGPoint HCObjcSendVoidMessageCGPoint = (HCObjcVoidMessageCGPoint)objc_msgSend;
typedef CGPoint (*HCObjcCGPointMessageVoid)(id, SEL);
static HCObjcCGPointMessageVoid HCObjcSendCGPointMessageVoid = (HCObjcCGPointMessageVoid)objc_msgSend;
typedef CGPoint (*HCObjcCGPointMessageId)(id, SEL, id);
static HCObjcCGPointMessageId HCObjcSendCGPointMessageId = (HCObjcCGPointMessageId)objc_msgSend;
typedef CGPoint (*HCObjcCGPointMessageCGPoint)(id, SEL, CGPoint);
static HCObjcCGPointMessageCGPoint HCObjcSendCGPointMessageCGPoint = (HCObjcCGPointMessageCGPoint)objc_msgSend;
typedef CGPoint (*HCObjcCGPointMessageCGPointId)(id, SEL, CGPoint, id);
static HCObjcCGPointMessageCGPointId HCObjcSendCGPointMessageCGPointId = (HCObjcCGPointMessageCGPointId)objc_msgSend;
typedef id (*HCObjcIdMessageCGPoint)(id, SEL, CGPoint);
static HCObjcIdMessageCGPoint HCObjcSendIdMessageCGPoint = (HCObjcIdMessageCGPoint)objc_msgSend;

typedef void (*HCObjcVoidMessageCGSize)(id, SEL, CGSize);
static HCObjcVoidMessageCGSize HCObjcSendVoidMessageCGSize = (HCObjcVoidMessageCGSize)objc_msgSend;
typedef CGSize (*HCObjcCGSizeMessageVoid)(id, SEL);
static HCObjcCGSizeMessageVoid HCObjcSendCGSizeMessageVoid = (HCObjcCGSizeMessageVoid)objc_msgSend_stret;
typedef CGSize (*HCObjcCGSizeMessageCGSize)(id, SEL);
static HCObjcCGSizeMessageCGSize HCObjcSendCGSizeMessageCGSize = (HCObjcCGSizeMessageCGSize)objc_msgSend_stret;
typedef CGSize (*HCObjcCGSizeMessageId)(id, SEL, id);
static HCObjcCGSizeMessageId HCObjcSendCGSizeMessageId = (HCObjcCGSizeMessageId)objc_msgSend_stret;
typedef id (*HCObjcIdMessageCGSize)(id, SEL, CGSize);
static HCObjcIdMessageCGSize HCObjcSendIdMessageCGSize = (HCObjcIdMessageCGSize)objc_msgSend;

typedef void (*HCObjcVoidMessageCGRect)(id, SEL, CGRect);
static HCObjcVoidMessageCGRect HCObjcSendVoidMessageCGRect = (HCObjcVoidMessageCGRect)objc_msgSend;
typedef CGRect (*HCObjcCGRectMessageVoid)(id, SEL);
static HCObjcCGRectMessageVoid HCObjcSendCGRectMessageVoid = (HCObjcCGRectMessageVoid)objc_msgSend_stret;
typedef CGRect (*HCObjcCGRectMessageCGRect)(id, SEL, CGRect);
static HCObjcCGRectMessageCGRect HCObjcSendCGRectMessageCGRect = (HCObjcCGRectMessageCGRect)objc_msgSend_stret;
typedef CGRect (*HCObjcCGRectMessageId)(id, SEL, id);
static HCObjcCGRectMessageId HCObjcSendCGRectMessageId = (HCObjcCGRectMessageId)objc_msgSend_stret;
typedef id (*HCObjcIdMessageCGRect)(id, SEL, CGRect);
static HCObjcIdMessageCGRect HCObjcSendIdMessageCGRect = (HCObjcIdMessageCGRect)objc_msgSend;

typedef id (*HCObjcVoidMessageCGRectBool)(id, SEL, CGRect, bool);
static HCObjcVoidMessageCGRectBool HCObjcSendVoidMessageCGRectBool = (HCObjcVoidMessageCGRectBool)objc_msgSend;

typedef CGRect (*HCObjcCGRectMessageCGRectInt)(id, SEL, CGRect, int);
static HCObjcCGRectMessageCGRectInt HCObjcSendCGRectMessageCGRectInt = (HCObjcCGRectMessageCGRectInt)objc_msgSend_stret;

typedef id (*HCObjcIdMessageCGRectIntIntBool)(id, SEL, CGRect, int, int, bool);
static HCObjcIdMessageCGRectIntIntBool HCObjcSendIdMessageCGRectIntIntBool = (HCObjcIdMessageCGRectIntIntBool)objc_msgSend;

typedef id (*HCObjcIdMessageCGFloatCGFloatCGFloatCGFloat)(id, SEL, CGFloat, CGFloat, CGFloat, CGFloat);
static HCObjcIdMessageCGFloatCGFloatCGFloatCGFloat HCObjcSendIdMessageCGFloatCGFloatCGFloatCGFloat = (HCObjcIdMessageCGFloatCGFloatCGFloatCGFloat)objc_msgSend;

typedef id (*HCObjcBoolMessageCGFloatPtrCGFloatPtrCGFloatPtrCGFloatPtr)(id, SEL, CGFloat*, CGFloat*, CGFloat*, CGFloat*);
static HCObjcBoolMessageCGFloatPtrCGFloatPtrCGFloatPtrCGFloatPtr HCObjcSendBoolMessageCGFloatPtrCGFloatPtrCGFloatPtrCGFloatPtr = (HCObjcBoolMessageCGFloatPtrCGFloatPtrCGFloatPtrCGFloatPtr)objc_msgSend;

typedef void (*HCObjcVoidMessageIdSELIdId)(id, SEL, id, SEL, id, id);
static HCObjcVoidMessageIdSELIdId HCObjcSendVoidMessageIdSELIdId = (HCObjcVoidMessageIdSELIdId)objc_msgSend;

typedef void (*HCObjcVoidMessageIdIdId)(id, SEL, id, id, id);
static HCObjcVoidMessageIdIdId HCObjcSendVoidMessageIdIdId = (HCObjcVoidMessageIdIdId)objc_msgSend;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Message Sending Convenience
//----------------------------------------------------------------------------------------------------------------------------------
static inline id HCObjcSendRetain(id object) {
    return HCObjcSendIdMessageVoid(object, sel_getUid("retain"));
}

static inline void HCObjcSendRelease(id object) {
    HCObjcSendVoidMessageVoid(object, sel_getUid("release"));
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Conversion Convenience
//----------------------------------------------------------------------------------------------------------------------------------
static inline HCPoint HCPointMakeWithCGPoint(CGPoint point) {
    return HCPointMake(point.x, point.y);
}

static inline CGPoint CGPointMakeWithHCPoint(HCPoint point) {
    return (CGPoint){.x = point.x, .y = point.y};
}

static inline HCSize HCSizeMakeWithCGSize(CGSize size) {
    return HCSizeMake(size.width, size.height);
}

static inline CGSize CGSizeMakeWithHCSize(HCSize size) {
    return (CGSize){.width = size.width, .height = size.height};
}

static inline HCRectangle HCRectangleMakeWithCGRect(CGRect rect) {
    return HCRectangleMake(HCPointMake(rect.origin.x, rect.origin.y), HCSizeMake(rect.size.width, rect.size.height));
}

static inline CGRect CGRectMakeWithHCRectangle(HCRectangle rect) {
    return (CGRect){.origin = {.x = rect.origin.x, .y = rect.origin.y}, .size = {.width = rect.size.width, .height = rect.size.height}};
}

static inline HCColor HCColorMakeWithNSColor(id nsColor) {
    HCColor hcColor = HCColorInvalid;
    HCObjcSendBoolMessageCGFloatPtrCGFloatPtrCGFloatPtrCGFloatPtr(nsColor, sel_getUid("getRed:green:blue:alpha:"), &hcColor.r, &hcColor.g, &hcColor.b, &hcColor.a);
    return hcColor;
}

static inline id NSColorAllocInitWithHCColor(HCColor hcColor) {
    // TODO: Autorelease pool?
    id nsColor = HCObjcSendIdMessageCGFloatCGFloatCGFloatCGFloat((id)objc_getClass("NSColor"), sel_getUid("colorWithRed:green:blue:alpha:"), hcColor.r, hcColor.g, hcColor.b, hcColor.a);
    return HCObjcSendRetain(nsColor);
}

static inline HCStringRef HCStringCreateWithNSString(id nsString) {
    const char* utf8String = (const char*)HCObjcSendIdMessageVoid(nsString, sel_getUid("UTF8String"));
    return HCStringCreateWithCString(utf8String);
}

static inline id NSStringAllocInitWithHCString(HCStringRef hcString) {
    const char* utf8String = HCStringAsCString(hcString);
    // TODO: Autorelease pool?
    id nsString = HCObjcSendIdMessageId((id)objc_getClass("NSString"), sel_getUid("stringWithUTF8String:"), (id)utf8String);
    return HCObjcSendRetain(nsString);
}

static inline id NSStringAllocInitWithCString(const void* cString) {
    // TODO: Autorelease pool?
    id nsString = HCObjcSendIdMessageId((id)objc_getClass("NSString"), sel_getUid("stringWithUTF8String:"), (id)cString);
    return HCObjcSendRetain(nsString);
}

#endif /* HCObjC_h */
