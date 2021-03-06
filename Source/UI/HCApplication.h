//
//  HCApplication.h
//  HollowMac
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCApplication_h
#define HCApplication_h

#include "../../HollowCore/Source/HollowCore.h"
#include "HCMenu.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCApplicationType;
typedef struct HCApplication* HCApplicationRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef void (*HCApplicationReadyCallback)(void* context, HCApplicationRef application);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCApplicationRef HCApplicationCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCApplicationIsEqual(HCApplicationRef self, HCApplicationRef other);
HCInteger HCApplicationHashValue(HCApplicationRef self);
void HCApplicationPrint(HCApplicationRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCApplicationMenuCount(HCApplicationRef self);
HCMenuRef HCApplicationMenuAtIndexRetained(HCApplicationRef self, HCInteger index);
void HCApplicationAddMenu(HCApplicationRef self, HCMenuRef menu);
void HCApplicationRemoveMenu(HCApplicationRef self, HCInteger index);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCApplicationRun(HCApplicationRef self, HCApplicationReadyCallback readyCallback, void* readyCallbackContext);
void HCApplicationTerminate(HCApplicationRef self);

#endif /* HCApplication_h */
