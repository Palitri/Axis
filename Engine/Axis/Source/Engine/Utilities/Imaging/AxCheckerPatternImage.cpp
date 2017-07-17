//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxCheckerPatternImage.h"


AxCheckerPatternImage::AxCheckerPatternImage(AxImage &result, int width, int height, int squareSize, AxVector4 &color1, AxVector4 &color2)
{
	if (squareSize < 1)
		squareSize = 1;

	result.SetSize(width, height, 1);

	float c1[4], c2[4];
	c1[0] = color1.x;
	c1[1] = color1.y;
	c1[2] = color1.z;
	c1[3] = color1.w;
	c2[0] = color2.x;
	c2[1] = color2.y;
	c2[2] = color2.z;
	c2[3] = color2.w;

	unsigned int color1Value = result.pixelFormat.SetRGBAUnitValues(c1);
	unsigned int color2Value = result.pixelFormat.SetRGBAUnitValues(c2);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			result.SetPixel(x, y, 0, ((x / squareSize) % 2) == ((y / squareSize) % 2) ? color1Value : color2Value);
		}
	}
}


AxCheckerPatternImage::~AxCheckerPatternImage(void)
{
}
