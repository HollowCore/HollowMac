//
//  HCMenu_Internal.h
//  HollowMac
//
//  Created by Matt Stoker on 10/31/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCMenu_Internal_h
#define HCMenu_Internal_h

#include "../../HollowCore/Source/Core/HCObject_Internal.h"
#include "HCMac.h"
#include "HCMenu.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCMenu {
    HCObject base;
    id nsMenuItem;
    id eventReceiver;
    HCMenuClickFunction clickCallback;
    void* clickContext;
} HCMenu;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCMenuRef HCMenuCreateWithNSMenuItem(id nsMenuItem);
void HCMenuInit(void* memory, id nsMenuItem);
void HCMenuDestroy(HCMenuRef self);

#endif /* HCMenu_Internal_h */
