#pragma once

#include "..\..\AxGlobals.h"

#include "AxVertex.h"

class AXDLLCLASS AxIntersectionInfo
{
public:
	AxVertex point;
    float distance;
    int primitiveIndex;
	bool hasIntersected;

	AxIntersectionInfo(void);
	~AxIntersectionInfo(void);
};

