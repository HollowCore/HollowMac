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
    HCViewRef view = HCViewCreate(40, 80);
    ASSERT_EQUAL(HCViewWidth(view), 40);
    ASSERT_EQUAL(HCViewHeight(view), 80);
    HCRelease(view);
}

CTEST(HCView, EqualHash) {
    HCViewRef a = HCViewCreate(40, 80);
    HCViewRef b = HCViewCreate(40, 60);
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCView, Print) {
    HCViewRef view = HCViewCreate(40, 80);
    HCViewPrint(view, stdout); // TODO: Not to stdout
    HCRelease(view);
}

CTEST(HCView, Dimensions) {
    HCViewRef view = HCViewCreate(40, 80);
    ASSERT_EQUAL(HCViewWidth(view), 40);
    ASSERT_EQUAL(HCViewHeight(view), 80);
    HCRelease(view);
}
