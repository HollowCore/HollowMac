//
//  HCApplication.h
//  HollowMac
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCApplication_h
#define HCApplication_h

#include "../../HollowCore/Source/Core/HCObject.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCApplicationType;
typedef struct HCApplication* HCApplicationRef;

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

#endif /* HCApplication_h */
