//
//  HCButton_Internal.h
//  HollowMac
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCButton_Internal_h
#define HCButton_Internal_h

#include "../../HollowCore/Source/Core/HCObject_Internal.h"
#include "HCMac.h"
#include "HCButton.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCButton {
    HCObject base;
    HCInteger width;
    HCInteger height;
    id button;
    id eventReceiver;
} HCButton;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCButtonInit(void* memory, HCInteger width, HCInteger height);
void HCButtonDestroy(HCButtonRef self);

#endif /* HCButton_Internal_h */
