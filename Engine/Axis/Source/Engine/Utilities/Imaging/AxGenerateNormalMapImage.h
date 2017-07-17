#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Media\Image\AxImage.h"

#include "..\..\..\Engine\Primitive\AxVector4.h"

class AXDLLCLASS AxGenerateNormalMapImage
{
public:
	// Generates a normal map from an image, based on the X and Y slopes of the color values of the image, using the Sobel-Feldman filtering
	// Useful for generation of normal maps from bump maps, given a proper light vector is provided.
	// Can accept the same image as a souce and result
	// The first depth layer of the image (z=0) is processed
	// result The image which is to hold the resulting normal map
	// source The image which is to be used as a source for generating the normal map. Ideally, it is a height map
	// emphasis An additional parameter which is to provide control over the roughness of the relief. Default values is 0.0, while 1.0 produces an over-extruded map. Smaller values lessen the effect
	AxGenerateNormalMapImage(AxImage &result, AxImage &source, AxVector4 &colorFilter, float emphasis = 0.0f);
	~AxGenerateNormalMapImage(void);
};

