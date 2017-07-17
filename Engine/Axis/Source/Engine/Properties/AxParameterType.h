#pragma once

#include "..\..\AxGlobals.h"

enum AxParameterType
{
    AxParameterType_None = 0,
    AxParameterType_Int = 1,
    AxParameterType_Float = 2,
    AxParameterType_Bool = 3,
    AxParameterType_Color = 4,
    AxParameterType_ReferenceVector3 = 1001,
    AxParameterType_ReferenceTexture = 1002,
    AxParameterType_ReferenceCamera = 1003,
    AxParameterType_ReferenceMechanism = 1004,
    AxParameterType_ReferenceTransform = 1005,
	AxParameterType_ReferenceSound = 1006,
	AxParameterType_ReferenceMesh = 1007
};