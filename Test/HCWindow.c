//
//  HCWindow.c
//  Test
//
//  Created by Matt Stoker on 2/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowMac.h"

CTEST(HCWindow, Creation) {
    HCWindowRef window = HCWindowCreate();
    ASSERT_NOT_NULL(HCWindowContentView(window));
    HCRelease(window);
}

CTEST(HCWindow, EqualHash) {
    HCWindowRef a = HCWindowCreate();
    HCWindowRef b = HCWindowCreate();
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCWindow, Print) {
    HCWindowRef window = HCWindowCreate();
    HCWindowPrint(window, stdout); // TODO: Not to stdout
    HCRelease(window);
}

CTEST(HCWindow, Geometry) {
    HCWindowRef window = HCWindowCreate();
    ASSERT_TRUE(HCWindowOriginLocation(window) == HCWindowCoordinateLocationLowerLeft);
    
    HCRectangle originalFrame = HCRectangleMake(HCPointMake(10.0, 20.0), HCSizeMake(100.0, 50.0));
    
    HCWindowSetFrame(window, originalFrame);
    ASSERT_TRUE(HCPointIsEqual(HCWindowCenter(window), HCPointMake(HCRectangleMidX(originalFrame), HCRectangleMidY(originalFrame))));
    ASSERT_TRUE(HCPointIsEqual(HCWindowOrigin(window), originalFrame.origin));
    ASSERT_TRUE(HCSizeIsEqual(HCWindowSize(window), originalFrame.size));
    ASSERT_TRUE(HCRectangleIsEqual(HCWindowFrame(window), originalFrame));
    
    HCPoint offsetOrigin = HCPointOffset(HCWindowOrigin(window), -5.0, 10.0);
    HCWindowSetOrigin(window, offsetOrigin);
    ASSERT_TRUE(HCPointIsEqual(HCWindowCenter(window), HCPointOffset(offsetOrigin, HCWindowSize(window).width * 0.5, HCWindowSize(window).height * 0.5)));
    ASSERT_TRUE(HCPointIsEqual(HCWindowOrigin(window), offsetOrigin));
    ASSERT_TRUE(HCSizeIsEqual(HCWindowSize(window), originalFrame.size));
    ASSERT_TRUE(HCRectangleIsEqual(HCWindowFrame(window), HCRectangleOffset(originalFrame, offsetOrigin.x - HCRectangleMinX(originalFrame), offsetOrigin.y - HCRectangleMinY(originalFrame))));
    ASSERT_TRUE(HCPointIsEqual(HCWindowFrame(window).origin, offsetOrigin));
    HCWindowSetFrame(window, originalFrame);
    
    HCPoint offsetCenter = HCPointOffset(HCWindowCenter(window), 20.0, -7.0);
    HCWindowSetCenter(window, offsetCenter);
    ASSERT_TRUE(HCPointIsEqual(HCWindowCenter(window), offsetCenter));
    ASSERT_TRUE(HCPointIsEqual(HCWindowOrigin(window), HCPointOffset(offsetCenter, HCWindowSize(window).width * -0.5, HCWindowSize(window).height * -0.5)));
    ASSERT_TRUE(HCSizeIsEqual(HCWindowSize(window), originalFrame.size));
    ASSERT_TRUE(HCRectangleIsEqual(HCWindowFrame(window), HCRectangleOffset(originalFrame, offsetCenter.x - HCRectangleMidX(originalFrame), offsetCenter.y - HCRectangleMidY(originalFrame))));
    ASSERT_TRUE(HCPointIsEqual(HCPointMake(HCRectangleMidX(HCWindowFrame(window)), HCRectangleMidY(HCWindowFrame(window))), offsetCenter));
    HCWindowSetFrame(window, originalFrame);
    
    HCSize changedSize = HCSizeIncrease(HCWindowSize(window), 40.0, -9.0);
    HCWindowSetSize(window, changedSize);
    ASSERT_TRUE(HCPointIsEqual(HCWindowCenter(window), HCPointMake(originalFrame.origin.x + changedSize.width * 0.5, originalFrame.origin.y + changedSize.height * 0.5)));
    ASSERT_TRUE(HCPointIsEqual(HCWindowOrigin(window), originalFrame.origin));
    ASSERT_TRUE(HCSizeIsEqual(HCWindowSize(window), changedSize));
    ASSERT_TRUE(HCRectangleIsEqual(HCWindowFrame(window), HCRectangleMake(originalFrame.origin, changedSize)));
    
    HCRelease(window);
}

CTEST(HCWindow, Display) {
    HCWindowRef window = HCWindowCreate();
    HCWindowDisplay(window);
    ASSERT_NOT_NULL(HCWindowContentView(window));
    HCRelease(window);
}
