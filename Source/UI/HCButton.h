//
//  HCButton.h
//  Test
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCButton_h
#define HCButton_h

#include "HCView.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCButtonType;
typedef struct HCButton* HCButtonRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef void (*HCButtonClickFunction)(void* context, HCButtonRef button);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCButtonRef HCButtonCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCButtonIsEqual(HCButtonRef self, HCButtonRef other);
HCInteger HCButtonHashValue(HCButtonRef self);
void HCButtonPrint(HCButtonRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCStringRef HCButtonTitleRetained(HCButtonRef self);
void HCButtonSetTitle(HCButtonRef self, HCStringRef title);
HCButtonClickFunction HCButtonClickCallback(HCButtonRef self);
void HCButtonSetClickCallback(HCButtonRef self, HCButtonClickFunction callback, void* context);

#endif /* HCButton_h */
