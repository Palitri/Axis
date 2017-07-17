#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "SrARGB.h"
#include "SrARGBInt.h"
#include "SrVector2Int.h"

struct SrShadingParameters
{
    int fogStart, fogEnd;
    SrARGB fogColor, basicLightColor, tintColor;
    float refractionFactor;
    SrARGBInt globalAmbient;
    int cellDiffuseSegmentation, cellSpecularSegmentation;
    int cellEnvMapSegmentation;
	SrVector2Int texTransformVector;
};