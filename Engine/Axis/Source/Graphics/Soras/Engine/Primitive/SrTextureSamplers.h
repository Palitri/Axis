#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "SrARGBInt.h"
#include "..\SrRenderThread.h"

typedef void (*SrTextureSampler)(SrRenderThread *renderThread, int tu, int tv, SrARGBInt &color);

class AXDLLCLASS SrTextureSamplers
{
public:
	static void Point(SrRenderThread *renderThread, int tu, int tv, SrARGBInt &color);
	static void Linear(SrRenderThread *renderThread, int tu, int tv, SrARGBInt &color);
};