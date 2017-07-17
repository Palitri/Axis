//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "SrTextureSamplers.h"

#include "..\SrRenderThread.h"

#include "SrUnit.h"
#include "SrARGB.h"

void SrTextureSamplers::Point(SrRenderThread *renderThread, int tu, int tv, SrARGBInt &color)
{
	tu = tu & (SrUnit::high - 1);
    tv = tv & (SrUnit::high - 1);

    int tx = (renderThread->shaderReg_Texture->highX * tu) >> SrUnit::bitCount;
    int ty = (renderThread->shaderReg_Texture->highY * tv) >> SrUnit::bitCount;

    int index = (ty * renderThread->shaderReg_Texture->width + tx) << 2;

    unsigned char *component = &renderThread->shaderReg_Texture->data[index];
    color.b = *(component++);
    color.g = *(component++);
	color.r = *(component++);
    color.a = *(component);
}

void SrTextureSamplers::Linear(SrRenderThread *renderThread, int tu, int tv, SrARGBInt &color)
{
	tu = tu & (SrUnit::high - 1);
    tv = tv & (SrUnit::high - 1);

    int tx = renderThread->shaderReg_Texture->highX * tu;
    int ty = renderThread->shaderReg_Texture->highY * tv;
    int fx = tx & (SrUnit::high - 1);
    int fy = ty & (SrUnit::high - 1);
    tx = tx >> SrUnit::bitCount;
    ty = ty >> SrUnit::bitCount;

    int index = (ty * renderThread->shaderReg_Texture->width + tx) << 2;

    SrARGB color1, color2;
    unsigned char *component = &renderThread->shaderReg_Texture->data[index];
    color1.b = *(component++);
    color1.g = *(component++);
    color1.r = *(component++);
    color1.a = *(component++);
    color2.b = *(component++);
    color2.g = *(component++);
    color2.r = *(component++);
    color2.a = *(component++);

	SrARGBInt color3, color4;
    color3.a = color1.a + (((color2.a - color1.a) * fx) >> SrUnit::bitCount);
    color3.r = color1.r + (((color2.r - color1.r) * fx) >> SrUnit::bitCount);
    color3.g = color1.g + (((color2.g - color1.g) * fx) >> SrUnit::bitCount);
    color3.b = color1.b + (((color2.b - color1.b) * fx) >> SrUnit::bitCount);

    component += (renderThread->shaderReg_Texture->width << 2) - 8;

    color1.b = *(component++);
    color1.g = *(component++);
    color1.r = *(component++);
    color1.a = *(component++);
    color2.b = *(component++);
    color2.g = *(component++);
    color2.r = *(component++);
    color2.a = *(component);

    color4.a = color1.a + (((color2.a - color1.a) * fx) >> SrUnit::bitCount);
    color4.r = color1.r + (((color2.r - color1.r) * fx) >> SrUnit::bitCount);
    color4.g = color1.g + (((color2.g - color1.g) * fx) >> SrUnit::bitCount);
    color4.b = color1.b + (((color2.b - color1.b) * fx) >> SrUnit::bitCount);

    color.a = color3.a + (((color4.a - color3.a) * fy) >> SrUnit::bitCount);
    color.r = color3.r + (((color4.r - color3.r) * fy) >> SrUnit::bitCount);
    color.g = color3.g + (((color4.g - color3.g) * fy) >> SrUnit::bitCount);
    color.b = color3.b + (((color4.b - color3.b) * fy) >> SrUnit::bitCount);
}