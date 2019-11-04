//
//  HCRasterView.c
//  Test
//
//  Created by Matt Stoker on 11/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowMac.h"

CTEST(HCRasterView, Creation) {
    HCRasterViewRef view = HCRasterViewCreate();
    ASSERT_TRUE(HCRectangleIsZero(HCViewFrame((HCViewRef)view)));
    HCRelease(view);
}

CTEST(HCRasterView, EqualHash) {
    HCRasterViewRef a = HCRasterViewCreate();
    HCRasterViewRef b = HCRasterViewCreate();
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCRasterView, Print) {
    HCRasterViewRef view = HCRasterViewCreate();
    HCRasterViewPrint(view, stdout); // TODO: Not to stdout
    HCRelease(view);
}

void HCRasterViewTestDraw(void* context, HCRasterViewRef view, HCRasterRef raster) {
    ASSERT_NOT_NULL(view);
    ASSERT_TRUE(context == (void*)0xDEADBEEF);
}

CTEST(HCRasterView, Draw) {
    HCRasterViewRef view = HCRasterViewCreate();
    HCRasterViewSetDrawCallback(view, HCRasterViewTestDraw, (void*)0xDEADBEEF);
    ASSERT_TRUE(HCRasterViewDrawCallback(view) == HCRasterViewTestDraw);
    HCRasterViewInvalidate(view);
    HCRelease(view);
}
