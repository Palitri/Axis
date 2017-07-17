#pragma once

#include "..\..\AxGlobals.h"

enum AxRenderState
{
    AxRenderState_AlphaBlendNone = 0,
    AxRenderState_AlphaBlendTransparency = 1,
    AxRenderState_AlphaBlendAdd = 2,
    AxRenderState_DepthEnable = 3,
    AxRenderState_DepthDisable = 4,
    AxRenderState_CullNone = 5,
    AxRenderState_CullBackFace = 6,
    AxRenderState_CullFrontFace = 7
};