//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxGenerateNormalMapImage.h"

#include "..\..\..\Tools\AxMath.h"

// Sobel-Feldman filter for the X slope, left-to-right:
// 1.0f,		0.0f,	-1.0f,
// 2.0f,		0.0f,	-2.0f,
// 1.0f,		0.0f,	-1.0f
// That is, 11 - 13 + 2 * (21 - 23) + 31 - 33

// Sobel-Feldman filter for the Y slope, bottom-to-top:
// -1.0f,		-2.0f,	-1.0f,
// 0.0f,		0.0f,	0.0f,
// 1.0f,		2.0f,	1.0f
// That is, -(11 - 31 + 2 * (12 - 32) + 13 - 33) = 33 - 13 + 2 * (32 - 12) + 31 - 11

AxGenerateNormalMapImage::AxGenerateNormalMapImage(AxImage &result, AxImage &source, AxVector4 &colorFilter, float emphasis)
{
	AxImage *resultBuffer;
	bool isBuffered = &result == &source;

	float filter[4];
	filter[0] = colorFilter.x;
	filter[1] = colorFilter.y;
	filter[2] = colorFilter.z;
	filter[3] = colorFilter.w;

	if (isBuffered)
	{
		resultBuffer = new AxImage();
		resultBuffer->pixelFormat = source.pixelFormat;
	}
	else
		resultBuffer = &result;

	resultBuffer->SetSize(AxMath::Max(resultBuffer->width, source.width), AxMath::Max(resultBuffer->height, source.height), AxMath::Max(resultBuffer->depth, 1));

	for (int y = 0; y < source.height; y++)
	{
		for (int x = 0; x < source.width; x++)
		{
			int x0 = x == 0 ? source.width - 1 : x - 1;
			int y0 = y == 0 ? source.height - 1 : y - 1;
			int x2 = (x + 1) % source.width;
			int y2 = (y + 1) % source.height;

			float p00[4], p01[4], p02[4], p10[4], p12[4], p20[4], p21[4], p22[4];
			source.pixelFormat.GetRGBAUnitValues(p00, source.GetPixel(x0,	y0,	0));
			source.pixelFormat.GetRGBAUnitValues(p01, source.GetPixel(x,	y0,	0));
			source.pixelFormat.GetRGBAUnitValues(p02, source.GetPixel(x2,	y0,	0));
			source.pixelFormat.GetRGBAUnitValues(p10, source.GetPixel(x0,	y,	0));
			source.pixelFormat.GetRGBAUnitValues(p12, source.GetPixel(x2,	y,	0));
			source.pixelFormat.GetRGBAUnitValues(p20, source.GetPixel(x0,	y2,	0));
			source.pixelFormat.GetRGBAUnitValues(p21, source.GetPixel(x,	y2,	0));
			source.pixelFormat.GetRGBAUnitValues(p22, source.GetPixel(x2,	y2,	0));

			float normal[4] = { 0.0f, 0.0f, 1.0f - emphasis, 1.0f };
			for (int c = 0; c < 4; c++)
			{
				// Accumulate the sobelX filter result for each channel in normal[0], which is the X slope and the sobelY filter result for each channel in normal[1], which is the Y slope
				normal[0] += (p00[c] - p02[c] + 2 * (p10[c] - p12[c]) + p20[c] - p22[c]) * filter[c];
				normal[1] += (p22[c] - p02[c] + 2 * (p21[c] - p01[c]) + p20[c] - p00[c]) * filter[c];

				normal[3] += p00[c] * filter[c];
			}

			// Normalize and transform from [-1, 1] to [0, 1]
			float k = 0.5f / AxMath::Sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
			normal[0] = normal[0] * k + 0.5f;
			normal[1] = normal[1] * k + 0.5f;
			normal[2] = normal[2] * k + 0.5f;
			
			normal[3] = normal[3] / (filter[0] + filter[1] + filter[2] + filter[3]);

			resultBuffer->SetPixel(x, y, 0, result.pixelFormat.SetChannelUnitValues(normal));
		}
	}

	if (isBuffered)
	{
		result.CopyPixels(*resultBuffer, 0, 0, 0, result.width, result.height, 1);
		delete resultBuffer;
	}
}


AxGenerateNormalMapImage::~AxGenerateNormalMapImage(void)
{
}
