//
//  HCWindow.c
//  Test
//
//  Created by Matt Stoker on 10/13/19.
//  Copyright © 2019 HollowCore. All rights reserved.
//

#include "HCWindow_Internal.h"
#include "../../../HollowCore/Source/Core/HCCore.h"
#include <math.h>
#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Type
//----------------------------------------------------------------------------------------------------------------------------------
const HCObjectTypeData HCWindowTypeDataInstance = {
    .base = {
        .ancestor = &HCObjectTypeDataInstance.base,
        .name = "HCWindow",
    },
    .isEqual = (void*)HCWindowIsEqual,
    .hashValue = (void*)HCWindowHashValue,
    .print = (void*)HCWindowPrint,
    .destroy = (void*)HCWindowDestroy,
};
HCType HCWindowType = (HCType)&HCWindowTypeDataInstance;

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Construction
//----------------------------------------------------------------------------------------------------------------------------------
HCWindowRef HCWindowCreate(HCInteger width, HCInteger height) {
    HCWindowRef self = calloc(sizeof(HCWindow), 1);
    HCWindowInit(self, width, height);
    return self;
}

void HCWindowInit(void* memory,  HCInteger width, HCInteger height) {
    HCObjectInit(memory);
    HCWindowRef self = memory;
    self->base.type = HCWindowType;
    self->width = width;
    self->height = height;
}

void HCWindowDestroy(HCWindowRef self) {
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Object Polymorphic Functions
//----------------------------------------------------------------------------------------------------------------------------------
HCBoolean HCWindowIsEqual(HCWindowRef self, HCWindowRef other) {
    if (self->width != other->width || self->height != other->height) {
        return false;
    }
    return true;
}

HCInteger HCWindowHashValue(HCWindowRef self) {
    return HCIntegerHashValue(self->width) ^ HCIntegerHashValue(self->height);
}

void HCWindowPrint(HCWindowRef self, FILE* stream) {
    fprintf(stream, "<%s@%p,size:%lix%li>", self->base.type->name, self, (long)self->width, (long)self->height);
}

//----------------------------------------------------------------------------------------------------------------------------------
// MARK: - Attributes
//----------------------------------------------------------------------------------------------------------------------------------
HCInteger HCWindowWidth(HCWindowRef self) {
    return self->width;
}

HCInteger HCWindowHeight(HCWindowRef self) {
    return self->height;
}
