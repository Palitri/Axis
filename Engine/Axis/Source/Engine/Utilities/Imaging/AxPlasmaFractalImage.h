#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Primitive\AxVector4.h"

#include "..\..\..\Media\Image\AxImage.h"

class AXDLLCLASS AxPlasmaFractalImage
{
private:
	static void SetPixel(AxImage &destination, int x, int y, AxVector4 &color);
	static AxVector4 GetPixel(AxImage &source, int x, int y);

	static AxVector4 GetDiamondSample(AxImage &source, int x, int y, int diamondSize);
	static AxVector4 GetSquareSample(AxImage &source, int x, int y, int squareSize);

	static AxVector4 GetRandomDeviation(AxVector4 color, float deviationRange);

public:
	AxPlasmaFractalImage(AxImage &result, int size, AxVector4 color, float roughness);
	virtual ~AxPlasmaFractalImage(void);
};

