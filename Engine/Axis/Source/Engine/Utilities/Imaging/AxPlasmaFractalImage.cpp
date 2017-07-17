//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPlasmaFractalImage.h"

#include "..\..\Utilities\AxMaths.h"
#include "..\..\..\Tools\AxMath.h"

AxPlasmaFractalImage::AxPlasmaFractalImage(AxImage &result, int size, AxVector4 color, float roughness)
{
	size = AxMaths::GetPowerOf2(size);
	result.SetSize(size, size, 1);
	AxPlasmaFractalImage::SetPixel(result, 0, 0, color);

	float randomDeviationRange = 1.0f;

	int sampleSize = size;
	while (sampleSize > 1)
	{
		int halfSample = sampleSize / 2;

		for (int y = halfSample; y < size; y += sampleSize)
		{
			for (int x = halfSample; x < size; x += sampleSize)
			{
				AxVector4 diamond = AxPlasmaFractalImage::GetRandomDeviation(AxPlasmaFractalImage::GetDiamondSample(result, x, y, halfSample), randomDeviationRange);
				AxPlasmaFractalImage::SetPixel(result, x, y, diamond);
			}
		}

		for (int y = halfSample; y < size; y += sampleSize)
		{
			for (int x = halfSample; x < size; x += sampleSize)
			{
				AxVector4 square;
				square = AxPlasmaFractalImage::GetRandomDeviation(AxPlasmaFractalImage::GetSquareSample(result, x, y - halfSample, halfSample), randomDeviationRange);
				AxPlasmaFractalImage::SetPixel(result, x, y - halfSample, square);

				square = AxPlasmaFractalImage::GetRandomDeviation(AxPlasmaFractalImage::GetSquareSample(result, x - halfSample, y, halfSample), randomDeviationRange);
				AxPlasmaFractalImage::SetPixel(result, x - halfSample, y, square);
			}
		}

		randomDeviationRange *= roughness;

		sampleSize /= 2;
	}
}


AxPlasmaFractalImage::~AxPlasmaFractalImage(void)
{
}


AxVector4 AxPlasmaFractalImage::GetDiamondSample(AxImage &source, int x, int y, int diamondSize)
{
	AxVector4 s11 = AxPlasmaFractalImage::GetPixel(source, x - diamondSize, y - diamondSize);
	AxVector4 s12 = AxPlasmaFractalImage::GetPixel(source, x + diamondSize, y - diamondSize);
	AxVector4 s21 = AxPlasmaFractalImage::GetPixel(source, x - diamondSize, y + diamondSize);
	AxVector4 s22 = AxPlasmaFractalImage::GetPixel(source, x + diamondSize, y + diamondSize);

	return s11.Add(s12).Add(s21).Add(s22).Scale(1.0f / 4.0f);
}

AxVector4 AxPlasmaFractalImage::GetSquareSample(AxImage &source, int x, int y, int squareSize)
{
	AxVector4 left = AxPlasmaFractalImage::GetPixel(source, x - squareSize, y);
	AxVector4 right = AxPlasmaFractalImage::GetPixel(source, x + squareSize, y);
	AxVector4 top = AxPlasmaFractalImage::GetPixel(source, x, y - squareSize);
	AxVector4 bottom = AxPlasmaFractalImage::GetPixel(source, x, y + squareSize);

	return left.Add(right).Add(top).Add(bottom).Scale(1.0f / 4.0f);
}

void AxPlasmaFractalImage::SetPixel(AxImage &destination, int x, int y, AxVector4 &color)
{
	x = x % destination.width;
	if (x < 0)
		x += destination.width;

	y = y % destination.height;
	if (y < 0)
		y += destination.height;

	float colorValues[4];
	colorValues[0] = color.x;
	colorValues[1] = color.y;
	colorValues[2] = color.z;
	colorValues[3] = color.w;

	destination.SetPixel(x, y, 0, destination.pixelFormat.SetRGBAUnitValues(colorValues));
}

AxVector4 AxPlasmaFractalImage::GetPixel(AxImage &source, int x, int y)
{
	x = x % source.width;
	if (x < 0)
		x += source.width;

	y = y % source.height;
	if (y < 0)
		y += source.height;

	float colorValues[4];
	source.pixelFormat.GetRGBAUnitValues(colorValues, source.GetPixel(x, y, 0));

	return AxVector4(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
}

AxVector4 AxPlasmaFractalImage::GetRandomDeviation(AxVector4 color, float deviationRange)
{
	AxVector4 result;
	result.x = AxMath::Trim(color.x - deviationRange / 2.0f + AxMath::Random() * deviationRange, 0.0f, 1.0f);
	result.y = AxMath::Trim(color.y - deviationRange / 2.0f + AxMath::Random() * deviationRange, 0.0f, 1.0f);
	result.z = AxMath::Trim(color.z - deviationRange / 2.0f + AxMath::Random() * deviationRange, 0.0f, 1.0f);
	result.w = color.w;
	return result;
}
