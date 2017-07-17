#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxFile3dsPrimitiveTypes.h"

#include "..\..\..\..\Tools\AxString.h"

class AXDLLCLASS AxFile3dsMaterial
{
public:
	AxString name;
	AxFile3dsColorByte ambient, diffuse, specular;
	float specularLevel, glossiness;
	bool hasAmbient, hasDiffuse, hasSpecular;
	AxString colorMap, specularMap, bumpMap, reflectionMap;

	AxFile3dsMaterial(void);
	~AxFile3dsMaterial(void);
};

