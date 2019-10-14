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
    HCWindowRef window = HCWindowCreate(1, 1);
    ASSERT_EQUAL(HCWindowWidth(window), 1);
    ASSERT_EQUAL(HCWindowHeight(window), 1);
    HCRelease(window);
}

CTEST(HCWindow, EqualHash) {
    HCWindowRef a = HCWindowCreate(1, 1);
    HCWindowRef b = HCWindowCreate(1, 1);
    ASSERT_TRUE(HCObjectIsEqual(a, b));
    ASSERT_EQUAL(HCObjectHashValue(a), HCObjectHashValue(a));
    ASSERT_EQUAL(HCObjectHashValue(a), HCObjectHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCWindow, Print) {
    HCWindowRef window = HCWindowCreate(1, 1);
    HCWindowPrint(window, stdout); // TODO: Not to stdout
    HCRelease(window);
}

CTEST(HCWindow, Dimensions) {
    HCWindowRef window = HCWindowCreate(100, 200);
    ASSERT_EQUAL(HCWindowWidth(window), 100);
    ASSERT_EQUAL(HCWindowHeight(window), 200);
    HCRelease(window);
}
