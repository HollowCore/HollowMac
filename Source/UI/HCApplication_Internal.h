//
//  HCApplication_Internal.h
//  HollowMac
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCApplication_Internal_h
#define HCApplication_Internal_h

#include "../../HollowCore/Source/Core/HCObject_Internal.h"
#include "HCMac.h"
#include "HCApplication.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCApplication {
    HCObject base;
    HCApplicationReadyCallback readyCallback;
    void* readyCallbackContext;
} HCApplication;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCApplicationInit(void* memory);
void HCApplicationDestroy(HCApplicationRef self);

#endif /* HCApplication_Internal_h */
