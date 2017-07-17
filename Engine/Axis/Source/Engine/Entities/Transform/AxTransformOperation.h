#pragma once

#include "..\..\..\AxGlobals.h"

// TODO: Consider moving each of these operations into mechanisms
// Consider making mechanisms, that work on the CURRENT object - that way moving a mechanism, let's say, from one Transform to another will immediately work on the second transform 
enum AxTransformOperation
{
    AxTransformOperation_Translate = 0,
    AxTransformOperation_RotateX = 1,
    AxTransformOperation_RotateY = 2,
    AxTransformOperation_RotateZ = 3,
    AxTransformOperation_Scale = 4,
    AxTransformOperation_TranslateAbsolute = 5,
    AxTransformOperation_RotateAbsoluteX = 6,
    AxTransformOperation_RotateAbsoluteY = 7,
    AxTransformOperation_RotateAbsoluteZ = 8,
    AxTransformOperation_ScaleAbsolute = 9,
    AxTransformOperation_Reflect = 10,
    AxTransformOperation_Skybox = 11
};