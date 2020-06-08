//
//  HCRasterView.h
//  Controls
//
//  Created by Matt Stoker on 11/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCRasterView_h
#define HCRasterView_h

#include "HCView.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCRasterViewType;
typedef struct HCRasterView* HCRasterViewRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef void (*HCRasterViewDrawFunction)(void* context, HCRasterViewRef view, HCRasterRef raster);
typedef void (*HCRasterViewMouseEventFunction)(void* context, HCRasterViewRef view, HCPoint location);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterViewRef HCRasterViewCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCRasterViewIsEqual(HCRasterViewRef self, HCRasterViewRef other);
HCInteger HCRasterViewHashValue(HCRasterViewRef self);
void HCRasterViewPrint(HCRasterViewRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterRef HCRasterViewRasterRetained(HCRasterViewRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Events
//----------------------------------------------------------------------------------------------------------------------------------
HCRasterViewDrawFunction HCRasterViewDrawCallback(HCRasterViewRef self);
void HCRasterViewSetDrawCallback(HCRasterViewRef self, HCRasterViewDrawFunction callback, void* context);
HCRasterViewMouseEventFunction HCRasterViewMouseDownCallback(HCRasterViewRef self);
void HCRasterViewSetMouseDownCallback(HCRasterViewRef self, HCRasterViewMouseEventFunction callback, void* context);
HCRasterViewMouseEventFunction HCRasterViewMouseMovedCallback(HCRasterViewRef self);
void HCRasterViewSetMouseMovedCallback(HCRasterViewRef self, HCRasterViewMouseEventFunction callback, void* context);
HCRasterViewMouseEventFunction HCRasterViewMouseUpCallback(HCRasterViewRef self);
void HCRasterViewSetMouseUpCallback(HCRasterViewRef self, HCRasterViewMouseEventFunction callback, void* context);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterViewDraw(HCRasterViewRef self);
void HCRasterViewInvalidate(HCRasterViewRef self);

#endif /* HCRasterView_h */
