//
//  HCView.c
//  Test
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowMac.h"

CTEST(HCView, Creation) {
    HCViewRef view = HCViewCreate();
    ASSERT_TRUE(HCRectangleIsZero(HCViewFrame(view)));
    HCRelease(view);
}

CTEST(HCView, EqualHash) {
    HCViewRef a = HCViewCreate();
    HCViewRef b = HCViewCreate();
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCView, Print) {
    HCViewRef view = HCViewCreate();
    HCViewPrint(view, stdout); // TODO: Not to stdout
    HCRelease(view);
}

CTEST(HCView, Geometry) {
    HCViewRef view = HCViewCreate();
    HCRectangle frame = HCRectangleMake(HCPointMake(10.0, 20.0), HCSizeMake(100.0, 50.0));
    HCViewSetFrame(view, frame);
    ASSERT_TRUE(HCRectangleIsEqual(HCViewFrame(view), frame));
    ASSERT_TRUE(HCSizeIsEqual(HCViewSize(view), frame.size));
    ASSERT_TRUE(HCPointIsEqual(HCViewCenter(view), HCPointMake(HCRectangleMidX(frame), HCRectangleMidY(frame))));
    HCRelease(view);
}

CTEST(HCView, Background) {
    HCViewRef view = HCViewCreate();
    HCColor color = HCColorMake(0.1, 0.2, 0.3, 0.4);
    HCViewSetBackgroundColor(view, color);
    ASSERT_TRUE(HCColorIsEqual(HCViewBackgroundColor(view), color));
}
