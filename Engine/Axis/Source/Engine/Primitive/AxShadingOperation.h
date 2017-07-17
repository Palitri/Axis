#pragma once

#include "..\..\AxGlobals.h"

enum AxShadingOperation
{
    AxShadingOperation_None = 0,
    AxShadingOperation_BasicShading = 1,
    AxShadingOperation_FlatColor = 2,
    AxShadingOperation_Tint = 3,
    AxShadingOperation_AmbientLighting = 4,
    AxShadingOperation_LightSource = 5,
    AxShadingOperation_VertexLighting = 6,
    AxShadingOperation_PixelLighting = 7,
    AxShadingOperation_ColorMap = 8,
    AxShadingOperation_ReflectionMap = 9,
    AxShadingOperation_RefractionMap = 10,
    AxShadingOperation_NormalMap = 11,
    AxShadingOperation_BlurMap = 12,
    AxShadingOperation_HBlur = 13,
    AxShadingOperation_VBlur = 14,
    AxShadingOperation_AlignScreenQuad = 15,
    AxShadingOperation_ProjectTexture = 16,
    AxShadingOperation_ProjectPicture = 17,
    AxShadingOperation_ShadowMapWriter = 18,
    AxShadingOperation_ShadowMap = 19,
    AxShadingOperation_TextureTransform = 20,
    AxShadingOperation_Skinning = 21,
    AxShadingOperation_Material = 22,
    AxShadingOperation_Fog = 23,
    AxShadingOperation_BumpMap = 24,
    AxShadingOperation_ChangeCulling = 25
};
