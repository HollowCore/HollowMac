//
//  HCButton_Internal.h
//  HollowMac
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCButton_Internal_h
#define HCButton_Internal_h

#include "HCView_Internal.h"
#include "HCButton.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCButton {
    HCView base;
    id eventReceiver;
    HCButtonClickFunction clickCallback;
    void* clickContext;
} HCButton;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCButtonRef HCButtonCreateWithView(id button);
void HCButtonInit(void* memory, id button);
void HCButtonDestroy(HCButtonRef self);

#endif /* HCButton_Internal_h */
