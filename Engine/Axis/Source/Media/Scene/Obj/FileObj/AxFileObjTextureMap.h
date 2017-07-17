#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\..\Tools\AxString.h"

#include "AxFileObjPrimitiveTypes.h"

class AxFileObjTextureMap
{
public:
	AxString fileName;
	AxFileObjVector3 offset, scale, turbulence;

	float brightness, contrastFactor, bumpFactor, mipMapSharpness;

	int resolution;

	bool clamp, blendU, blendV;

	AxFileObjTextureMapType mapType;
	AxFileObjTextureMapChannel mapChannel;

	AxFileObjTextureMap(void);
	~AxFileObjTextureMap(void);
};

