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
    HCButtonRef button = HCButtonCreate();
    ASSERT_TRUE(HCRectangleIsZero(HCViewFrame((HCViewRef)button)));
    HCRelease(button);
}

CTEST(HCButton, EqualHash) {
    HCButtonRef a = HCButtonCreate();
    HCButtonRef b = HCButtonCreate();
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCButton, Print) {
    HCButtonRef button = HCButtonCreate();
    HCButtonPrint(button, stdout); // TODO: Not to stdout
    HCRelease(button);
}

CTEST(HCButton, Title) {
    HCButtonRef button = HCButtonCreate();
    HCStringRef title = HCStringCreateWithCString("Great Stuff");
    HCButtonSetTitle(button, title);
    HCStringRef titleRetreived = HCButtonTitleRetained(button);
    ASSERT_TRUE(HCIsEqual(title, titleRetreived));
    HCRelease(title);
    HCRelease(titleRetreived);
    HCRelease(button);
}
