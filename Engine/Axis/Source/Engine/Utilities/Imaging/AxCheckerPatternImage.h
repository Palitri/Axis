#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Media\Image\AxImage.h"

#include "..\..\..\Engine\Primitive\AxVector4.h"

class AXDLLCLASS AxCheckerPatternImage
{
public:
	AxCheckerPatternImage(AxImage &result, int width, int height, int squareSize, AxVector4 &color1, AxVector4 &color2);
	~AxCheckerPatternImage(void);
};

