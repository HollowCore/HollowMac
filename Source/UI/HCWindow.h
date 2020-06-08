//
//  HCWindow.h
//  Test
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCWindow_h
#define HCWindow_h

#include "HCView.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
extern HCType HCWindowType;
typedef struct HCWindow* HCWindowRef;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Definitions
//----------------------------------------------------------------------------------------------------------------------------------
typedef enum HCWindowCoordinateLocation {
    HCWindowCoordinateLocationUpperLeft,
    HCWindowCoordinateLocationUpperRight,
    HCWindowCoordinateLocationLowerLeft,
    HCWindowCoordinateLocationLowerRight,
} HCWindowCoordinateLocation;

typedef void (*HCWindowResizeFunction)(void* context, HCWindowRef window, HCSize contentSize);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowRef HCWindowCreate(void);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCWindowIsEqual(HCWindowRef self, HCWindowRef other);
HCInteger HCWindowHashValue(HCWindowRef self);
void HCWindowPrint(HCWindowRef self, FILE* stream);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowCoordinateLocation HCWindowOriginLocation(HCWindowRef self);
HCPoint HCWindowCenter(HCWindowRef self);
void HCWindowSetCenter(HCWindowRef self, HCPoint center);
HCPoint HCWindowOrigin(HCWindowRef self);
void HCWindowSetOrigin(HCWindowRef self, HCPoint origin);
HCSize HCWindowSize(HCWindowRef self);
void HCWindowSetSize(HCWindowRef self, HCSize size);
HCRectangle HCWindowFrame(HCWindowRef self);
void HCWindowSetFrame(HCWindowRef self, HCRectangle frame);
HCRectangle HCWindowContentRectangle(HCWindowRef self);
HCViewRef HCWindowContentView(HCWindowRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Events
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowResizeFunction HCWindowResizeCallback(HCWindowRef self);
void HCWindowSetResizeCallback(HCWindowRef self, HCWindowResizeFunction callback, void* context);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Operations
//----------------------------------------------------------------------------------------------------------------------------------
void HCWindowDisplay(HCWindowRef self);

#endif /* HCWindow_h */
