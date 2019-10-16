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
typedef struct HCView {
    HCObject base;
    HCInteger width;
    HCInteger height;
    id view;
    HCListRef childViews;
} HCView;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCViewInit(void* memory, HCInteger width, HCInteger height);
void HCViewDestroy(HCViewRef self);

#endif /* HCView_Internal_h */
