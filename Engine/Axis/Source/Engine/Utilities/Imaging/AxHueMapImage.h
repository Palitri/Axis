#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Primitive\AxVector4.h"

#include "..\..\..\Media\Image\AxImage.h"

class AXDLLCLASS AxHueMapImage
{
public:
	// Generates an image mapping hue, saturation and lightness 
	// result An image which is to hold the resulting map
	// width The width in pixels of the image
	// height The height in pixels of the image
	// horizonatlHue A value, typically between [0, 1] which represents the horozontal transition of hue over the horizontal lines of the image, where 0 means it will remain constant and 1 means full transition
	// horizonatlSaturation A value, typically between [0, 1] which represents the horozontal transition of saturation over the horizontal lines of the image, where 0 means it will remain constant and 1 means full transition
	// horizonatlLightness A value, typically between [0, 1] which represents the horozontal transition of lightness over the horizontal lines of the image, where 0 means it will remain constant and 1 means full transition
	// verticalHue A value, typically between [0, 1] which represents the vertical transition of hue over the horizontal lines of the image, where 0 means it will remain constant and 1 means full transition
	// verticalSaturation A value, typically between [0, 1] which represents the vertical transition of saturation over the horizontal lines of the image, where 0 means it will remain constant and 1 means full transition
	// verticalLightness A value, typically between [0, 1] which represents the vertical transition of lightness over the horizontal lines of the image, where 0 means it will remain constant and 1 means full transition
	AxHueMapImage(AxImage &result, int width, int height, AxVector4 leftHSLA, AxVector4 rightHSLA, AxVector4 topHSLA, AxVector4 bottomHSLA, bool mirrorHorizontal, bool mirrorVertical);
	virtual ~AxHueMapImage(void);
};

