#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\..\Tools\AxString.h"

#include "AxFileObjPrimitiveTypes.h"
#include "AxFileObjTextureMap.h"

class AXDLLCLASS AxFileObjMaterial
{
public:
	AxString name;

	AxFileObjTextureMap *ambientMap, *diffuseMap, *specularColorMap, *specularHighlightMap, *alphaMap, *bumpMap, *displacementMap, *stencilMap, *reflectionMap;
	
	AxFileObjVector3 ambientColor, diffuseColor, specularColor;
	float specularFactor;
	float transparency;

	AxFileObjMaterial(void);
	~AxFileObjMaterial(void);
};

