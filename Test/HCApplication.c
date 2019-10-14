//
//  HCApplication.c
//  Test
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowMac.h"

CTEST(HCApplication, Creation) {
    HCApplicationRef window = HCApplicationCreate();
    HCRelease(window);
}

CTEST(HCApplication, EqualHash) {
    HCApplicationRef a = HCApplicationCreate();
    HCApplicationRef b = HCApplicationCreate();
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCApplication, Print) {
    HCApplicationRef window = HCApplicationCreate();
    HCApplicationPrint(window, stdout); // TODO: Not to stdout
    HCRelease(window);
}
