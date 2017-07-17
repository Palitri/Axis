//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxHueMapImage.h"

#include "..\..\Primitive\AxPixelFormat.h"

#include "..\..\..\Tools\AxMath.h"


AxHueMapImage::AxHueMapImage(AxImage &result, int width, int height, AxVector4 leftHSLA, AxVector4 rightHSLA, AxVector4 topHSLA, AxVector4 bottomHSLA, bool mirrorHorizontal, bool mirrorVertical)
{
	result.SetSize(width, height, 1);

    for (int y = 0; y < height; y++)
	{
		float v = (float)y / (float)height;
		if (mirrorVertical)
			v = 1.0f - AxMath::Abs(v * 2.0f - 1.0f);

        for (int x = 0; x < width; x++)
        {
			float u = (float)x / (float)width;
			if (mirrorHorizontal)
				u = 1.0f - AxMath::Abs(u * 2.0f - 1.0f);

			AxVector4 hsla = leftHSLA.Lerp(rightHSLA, u).Add(topHSLA.Lerp(bottomHSLA, v));

			float rgba[4], hsl[3];
            hsl[0] = hsla.x;
            hsl[1] = hsla.y;
            hsl[2] = hsla.z;
            AxPixelFormat::HSLToRGB(rgba, hsl);
			rgba[3] = hsla.w;
			
			result.SetPixel(x, y, 0, result.pixelFormat.SetRGBAUnitValues(rgba));
        }
	}
}


AxHueMapImage::~AxHueMapImage(void)
{
}
