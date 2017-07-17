#pragma once

#include "..\..\AxGlobals.h"

#include "AxVector3.h"

class AxMatrix;

class AXDLLCLASS AxBoundingVolume
{
public:
	AxVector3 center, box;
    float sphereRadius;

	AxBoundingVolume(void);
	~AxBoundingVolume(void);

	bool InsideViewFrustum(AxMatrix &viewFrustum);
};

