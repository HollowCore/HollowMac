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
    
    HCViewRef view = HCViewCreate(300, 100);
    HCViewAddChildView(HCWindowContentView(window), view);
    
    HCButtonRef button = HCButtonCreate(80, 40);
    HCViewAddChildView(view, (HCViewRef)button);
    
    HCWindowDisplay(window);
}

CTEST(Application, FullUI) {
    HCApplicationRef application = HCApplicationCreate();
    HCApplicationRun(application, ApplicationReady, NULL);
}
