#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "SrARGBInt.h"

struct SrMaterial
{
    SrARGBInt ambient, diffuse, specular;
    float gloss;
};