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
    HCRectangle originalFrame = HCRectangleMake(HCPointMake(10.0, 20.0), HCSizeMake(100.0, 50.0));
    
    HCViewSetFrame(view, originalFrame);
    ASSERT_TRUE(HCPointIsEqual(HCViewCenter(view), HCPointMake(HCRectangleMidX(originalFrame), HCRectangleMidY(originalFrame))));
    ASSERT_TRUE(HCPointIsEqual(HCViewOrigin(view), originalFrame.origin));
    ASSERT_TRUE(HCSizeIsEqual(HCViewSize(view), originalFrame.size));
    ASSERT_TRUE(HCRectangleIsEqual(HCViewFrame(view), originalFrame));
    
    HCPoint offsetOrigin = HCPointOffset(HCViewOrigin(view), -5.0, 10.0);
    HCViewSetOrigin(view, offsetOrigin);
    ASSERT_TRUE(HCPointIsEqual(HCViewCenter(view), HCPointOffset(offsetOrigin, HCViewSize(view).width * 0.5, HCViewSize(view).height * 0.5)));
    ASSERT_TRUE(HCPointIsEqual(HCViewOrigin(view), offsetOrigin));
    ASSERT_TRUE(HCSizeIsEqual(HCViewSize(view), originalFrame.size));
    ASSERT_TRUE(HCRectangleIsEqual(HCViewFrame(view), HCRectangleOffset(originalFrame, offsetOrigin.x - HCRectangleMinX(originalFrame), offsetOrigin.y - HCRectangleMinY(originalFrame))));
    ASSERT_TRUE(HCPointIsEqual(HCViewFrame(view).origin, offsetOrigin));
    HCViewSetFrame(view, originalFrame);
    
    HCPoint offsetCenter = HCPointOffset(HCViewCenter(view), 20.0, -7.0);
    HCViewSetCenter(view, offsetCenter);
    ASSERT_TRUE(HCPointIsEqual(HCViewCenter(view), offsetCenter));
    ASSERT_TRUE(HCPointIsEqual(HCViewOrigin(view), HCPointOffset(offsetCenter, HCViewSize(view).width * -0.5, HCViewSize(view).height * -0.5)));
    ASSERT_TRUE(HCSizeIsEqual(HCViewSize(view), originalFrame.size));
    ASSERT_TRUE(HCRectangleIsEqual(HCViewFrame(view), HCRectangleOffset(originalFrame, offsetCenter.x - HCRectangleMidX(originalFrame), offsetCenter.y - HCRectangleMidY(originalFrame))));
    ASSERT_TRUE(HCPointIsEqual(HCPointMake(HCRectangleMidX(HCViewFrame(view)), HCRectangleMidY(HCViewFrame(view))), offsetCenter));
    HCViewSetFrame(view, originalFrame);
    
    HCSize changedSize = HCSizeIncrease(HCViewSize(view), 40.0, -9.0);
    HCViewSetSize(view, changedSize);
    ASSERT_TRUE(HCPointIsEqual(HCViewCenter(view), HCPointMake(originalFrame.origin.x + changedSize.width * 0.5, originalFrame.origin.y + changedSize.height * 0.5)));
    ASSERT_TRUE(HCPointIsEqual(HCViewOrigin(view), originalFrame.origin));
    ASSERT_TRUE(HCSizeIsEqual(HCViewSize(view), changedSize));
    ASSERT_TRUE(HCRectangleIsEqual(HCViewFrame(view), HCRectangleMake(originalFrame.origin, changedSize)));
    
    HCRelease(view);
}

CTEST(HCView, Background) {
    HCViewRef view = HCViewCreate();
    HCColor color = HCColorMake(0.1, 0.2, 0.3, 0.4);
    HCViewSetBackgroundColor(view, color);
    ASSERT_TRUE(HCColorIsEqual(HCViewBackgroundColor(view), color));
}
