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
    HCApplicationRef application = HCApplicationCreate();
    HCRelease(application);
}

CTEST(HCApplication, EqualHash) {
    HCApplicationRef a = HCApplicationCreate();
    HCApplicationRef b = HCApplicationCreate();
    ASSERT_FALSE(HCIsEqual(a, b));
    ASSERT_EQUAL(HCHashValue(a), HCHashValue(a));
    ASSERT_EQUAL(HCHashValue(b), HCHashValue(b));
    HCRelease(a);
    HCRelease(b);
}

CTEST(HCApplication, Print) {
    HCApplicationRef application = HCApplicationCreate();
    HCApplicationPrint(application, stdout); // TODO: Not to stdout
    HCRelease(application);
}

void HCApplicationTestReadyCallback(void* context, HCApplicationRef application) {
    ASSERT_NOT_NULL(context);
    ASSERT_NOT_NULL(application);
    HCApplicationTerminate(application);
}

CTEST_SKIP(HCApplication, Run) {
    HCApplicationRef application = HCApplicationCreate();
    HCApplicationRun(application, HCApplicationTestReadyCallback, (void*)0xDEADBEEF);
    HCRelease(application);
}
