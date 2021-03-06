//
//  HCRasterView_Internal.h
//  HollowMac
//
//  Created by Matt Stoker on 11/3/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#ifndef HCRasterView_Internal_h
#define HCRasterView_Internal_h

#include "HCView_Internal.h"
#include "HCRasterView.h"

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
typedef struct HCRasterView {
    HCView base;
    HCRasterViewDrawFunction drawCallback;
    void* drawContext;
    HCRasterViewMouseEventFunction mouseDownCallback;
    void* mouseDownContext;
    HCRasterViewMouseEventFunction mouseMovedCallback;
    void* mouseMovedContext;
    HCRasterViewMouseEventFunction mouseUpCallback;
    void* mouseUpContext;
} HCRasterView;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterViewInit(void* memory);
void HCRasterViewDestroy(HCRasterViewRef self);

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Foundation
//----------------------------------------------------------------------------------------------------------------------------------
void HCRasterViewDrawRect(id nsView, SEL cmd, CGRect dirtyRect);
void HCRasterViewMouseDownEvent(id nsView, SEL cmd, id event);
void HCRasterViewMouseMovedEvent(id nsView, SEL cmd, id event);
void HCRasterViewMouseUpEvent(id nsView, SEL cmd, id event);

#endif /* HCRasterView_Internal_h */
