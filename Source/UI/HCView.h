//
//  HCView.h
//  Test
//
//  Created by Matt Stoker on 10/15/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCView_h
#define HCView_h

#include "../../HollowCore/Source/HollowCore.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef const struct HCViewTypeData {
    HCObjectTypeData base;
} HCViewTypeData;
extern HCType HCViewType;

typedef struct HCView* HCViewRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCViewRef HCViewCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCViewIsEqual(HCViewRef self, HCViewRef other);
HCInteger HCViewHashValue(HCViewRef self);
void HCViewPrint(HCViewRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCPoint HCViewCenter(HCViewRef self);
void HCViewSetCenter(HCViewRef self, HCPoint center);
HCPoint HCViewOrigin(HCViewRef self);
void HCViewSetOrigin(HCViewRef self, HCPoint origin);
HCSize HCViewSize(HCViewRef self);
void HCViewSetSize(HCViewRef self, HCSize size);
HCRectangle HCViewFrame(HCViewRef self);
void HCViewSetFrame(HCViewRef self, HCRectangle frame);
HCColor HCViewBackgroundColor(HCViewRef self);
void HCViewSetBackgroundColor(HCViewRef self, HCColor color);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Related Views
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCViewHasParentView(HCViewRef self);
HCViewRef HCViewParentViewRetained(HCViewRef self);
HCInteger HCViewChildViewCount(HCViewRef self);
HCViewRef HCViewChildViewAtIndexRetained(HCViewRef self, HCInteger index);
void HCViewAddChildView(HCViewRef self, HCViewRef child);
void HCViewRemoveChildView(HCViewRef self, HCInteger index);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCViewDraw(HCViewRef self);

#endif /* HCView_h */
