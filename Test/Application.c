//
//  Application.c
//  Test
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "ctest.h"
#include "../Source/HollowMac.h"

void ApplicationReady(void* context, HCApplicationRef application) {
    HCWindowRef window = HCWindowCreate(720, 480);
    HCWindowDisplay(window);
}

CTEST_SKIP(Application, FullUI) {
    HCApplicationRef application = HCApplicationCreate();
    HCApplicationRun(application, ApplicationReady, NULL);
}
