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
    id nsView;
} HCView;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCViewRef HCViewCreateWithNSView(id nsView);
void HCViewInit(void* memory, id nsView);
void HCViewDestroy(HCViewRef self);

#endif /* HCView_Internal_h */
