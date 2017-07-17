#pragma once

#include "..\..\AxGlobals.h"

#include "AxVector3.h"
#include "AxVector2.h"

class AXDLLCLASS AxVertex
{
public:
	AxVector3 position, normal;
    AxVector2 texCoords;

	AxVertex(void);
	~AxVertex(void);
};

