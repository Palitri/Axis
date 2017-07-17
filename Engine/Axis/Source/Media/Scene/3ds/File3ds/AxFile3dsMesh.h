#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxFile3dsBaseObject.h"

#include "AxFile3dsPrimitiveTypes.h"

#include "..\..\..\..\Tools\AxString.h"

class AXDLLCLASS AxFile3dsMesh :
	public AxFile3dsBaseObject
{
public:
    int numVertices, numFaces;
    AxFile3dsMeshVertex *vertices;
    AxFile3dsMeshFace *faces;
    AxFile3dsObjectTransformation localCoordSystem;
    AxString materialName;

	AxFile3dsMesh(void);
	~AxFile3dsMesh(void);

	void InverseTransformation();
};

