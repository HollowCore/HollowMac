//
//  HCMenu.h
//  HollowMac
//
//  Created by Matt Stoker on 10/31/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCMenu_h
#define HCMenu_h

#include "../../HollowCore/Source/HollowCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCMenuType;
typedef struct HCMenu* HCMenuRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef void (*HCMenuClickFunction)(void* context, HCMenuRef menu);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCMenuRef HCMenuCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCMenuIsEqual(HCMenuRef self, HCMenuRef other);
HCInteger HCMenuHashValue(HCMenuRef self);
void HCMenuPrint(HCMenuRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCStringRef HCMenuTitleRetained(HCMenuRef self);
void HCMenuSetTitle(HCMenuRef self, HCStringRef title);
HCMenuClickFunction HCMenuClickCallback(HCMenuRef self);
void HCMenuSetClickCallback(HCMenuRef self, HCMenuClickFunction callback, void* context);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Related Menus
//----------------------------------------------------------------------------------------------------------------------------------
HCMenuRef HCMenuParentMenuRetained(HCMenuRef self);
HCListRef HCMenuItemsRetained(HCMenuRef self);
HCInteger HCMenuChildMenuCount(HCMenuRef self);
HCMenuRef HCMenuChildMenuAtIndexRetained(HCMenuRef self, HCInteger index);
void HCMenuAddChildMenu(HCMenuRef self, HCMenuRef child);
void HCMenuRemoveChildMenu(HCMenuRef self, HCInteger index);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCMenuPerformClick(HCMenuRef self);

#endif /* HCMenu_h */
