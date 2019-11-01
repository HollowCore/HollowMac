//
//  HCMenu.c
//  Test
//
//  Created by Matt Stoker on 10/31/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowMac.h"

CTEST(HCMenu, Creation) {
    HCMenuRef menu = HCMenuCreate();
    HCRelease(menu);
}

CTEST(HCMenu, EqualHash) {
    HCMenuRef a = HCMenuCreate();
    HCMenuRef b = HCMenuCreate();
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCMenu, Print) {
    HCMenuRef menu = HCMenuCreate();
    HCMenuPrint(menu, stdout); // TODO: Not to stdout
    HCRelease(menu);
}

CTEST(HCMenu, Title) {
    HCMenuRef menu = HCMenuCreate();
    HCStringRef title = HCStringCreateWithCString("Great Stuff");
    HCMenuSetTitle(menu, title);
    HCStringRef titleRetreived = HCMenuTitleRetained(menu);
    ASSERT_TRUE(HCIsEqual(title, titleRetreived));
    HCRelease(title);
    HCRelease(titleRetreived);
    HCRelease(menu);
}
