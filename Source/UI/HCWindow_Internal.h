//
//  HCWindow_Internal.h
//  HollowCore
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCWindow_Internal_h
#define HCWindow_Internal_h

#include "../../HollowCore/Source/Core/HCObject_Internal.h"
#include "HCMac.h"
#include "HCWindow.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCWindow {
    HCObject base;
    id window;
    HCViewRef contentView;
} HCWindow;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowInit(void* memory);
void HCWindowDestroy(HCWindowRef self);

#endif /* HCWindow_Internal_h */
