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

CTEST(HCWindow, Display) {
    HCWindowRef window = HCWindowCreate();
    HCWindowDisplay(window);
    ASSERT_NOT_NULL(HCWindowContentView(window));
    HCRelease(window);
}
