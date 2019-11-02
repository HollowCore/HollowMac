//
//  main.c
//  Controls
//
//  Created by Matt Stoker on 11/2/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "../Source/HollowMac.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Application Callback Prototypes
//----------------------------------------------------------------------------------------------------------------------------------

void ApplicationReady(void* context, HCApplicationRef application);
void QuitClicked(void* context, HCMenuRef menu);
void ButtonClicked(void* context, HCButtonRef button);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Application Entry Point
//----------------------------------------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[]) {
    HCApplicationRef application = HCApplicationCreate();
    HCApplicationRun(application, ApplicationReady, NULL);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Application Callbacks
//----------------------------------------------------------------------------------------------------------------------------------

void ApplicationReady(void* context, HCApplicationRef application) {
    // Setup application menus
    HCMenuRef applicationMenu = HCMenuCreate();
    HCMenuRef quitMenuItem = HCMenuCreate();
    HCMenuSetTitle(quitMenuItem, HCStringCreateWithCString("Quit"));
    HCMenuSetClickCallback(quitMenuItem, QuitClicked, application);
    HCMenuAddChildMenu(applicationMenu, quitMenuItem);
    HCApplicationAddMenu(application, applicationMenu);
    
    // Setup application window and UI
    HCWindowRef window = HCWindowCreate();
    
    HCViewRef view = HCViewCreate();
    HCViewSetFrame(view, HCRectangleMake(HCPointMake(30.0, 50.0), HCSizeMake(300.0, 200.0)));
    HCViewSetBackgroundColor(view, HCColorRed);
    HCViewAddChildView(HCWindowContentView(window), view);
    
    HCButtonRef button = HCButtonCreate();
    HCViewSetFrame((HCViewRef)button, HCRectangleMake(HCPointMake(10.0, 20.0), HCSizeMake(80.0, 40.0)));
    HCButtonSetTitle(button, HCStringCreateWithCString("Click Me!"));
    HCButtonSetClickCallback(button, ButtonClicked, NULL);
    HCViewAddChildView(view, (HCViewRef)button);
    
    HCWindowDisplay(window);
}

void QuitClicked(void* context, HCMenuRef menu) {
    HCApplicationRef application = context;
    HCApplicationTerminate(application);
}

void ButtonClicked(void* context, HCButtonRef button) {
    printf("Clicked!\n");
    HCViewRef parent = HCViewParentViewRetained((HCViewRef)button);
    HCViewSetFrame(parent, HCRectangleOffset(HCViewFrame(parent), 1.0, 0.0));
    HCViewSetBackgroundColor(parent, HCColorGreen);
    HCRelease(parent);
}
