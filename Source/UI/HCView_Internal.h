//
//  HCView_Internal.h
//  HollowMac
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCView_Internal_h
#define HCView_Internal_h

#include "../../HollowCore/Source/Core/HCObject_Internal.h"
#include "HCMac.h"
#include "HCView.h"
#include "../../HollowCore/Source/Container/HCList.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCViewTypeData HCViewTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Instance Data
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCView {
    HCObject base;
    id view;
} HCView;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCViewRef HCViewCreateWithView(id view);
void HCViewInit(void* memory, id view);
void HCViewDestroy(HCViewRef self);

#endif /* HCView_Internal_h */
