//
//  HCButton.c
//  Test
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowMac.h"

CTEST(HCButton, Creation) {
    HCButtonRef button = HCButtonCreate(40, 80);
    ASSERT_EQUAL(HCButtonWidth(button), 40);
    ASSERT_EQUAL(HCButtonHeight(button), 80);
    HCRelease(button);
}

CTEST(HCButton, EqualHash) {
    HCButtonRef a = HCButtonCreate(40, 80);
    HCButtonRef b = HCButtonCreate(40, 60);
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCButton, Print) {
    HCButtonRef button = HCButtonCreate(40, 80);
    HCButtonPrint(button, stdout); // TODO: Not to stdout
    HCRelease(button);
}

CTEST(HCButton, Dimensions) {
    HCButtonRef button = HCButtonCreate(40, 80);
    ASSERT_EQUAL(HCButtonWidth(button), 40);
    ASSERT_EQUAL(HCButtonHeight(button), 80);
    HCRelease(button);
}
