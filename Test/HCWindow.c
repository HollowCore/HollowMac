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
    HCWindowRef window = HCWindowCreate(720, 480);
    ASSERT_EQUAL(HCWindowWidth(window), 720);
    ASSERT_EQUAL(HCWindowHeight(window), 480);
    HCRelease(window);
}

CTEST(HCWindow, EqualHash) {
    HCWindowRef a = HCWindowCreate(720, 480);
    HCWindowRef b = HCWindowCreate(640, 480);
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCWindow, Print) {
    HCWindowRef window = HCWindowCreate(720, 480);
    HCWindowPrint(window, stdout); // TODO: Not to stdout
    HCRelease(window);
}

CTEST(HCWindow, Dimensions) {
    HCWindowRef window = HCWindowCreate(720, 480);
    ASSERT_EQUAL(HCWindowWidth(window), 720);
    ASSERT_EQUAL(HCWindowHeight(window), 480);
    HCRelease(window);
}
