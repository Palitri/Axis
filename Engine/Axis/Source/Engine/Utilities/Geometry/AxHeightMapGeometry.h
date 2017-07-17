#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Media\Image\AxImage.h"

#include "AxPlaneGeometry.h"

class AXDLLCLASS AxHeightMapGeometry
{
public:
	AxHeightMapGeometry(AxDeviceMesh &result, AxImage &map, AxVector4 &colorFilter, float heightScaling);
	~AxHeightMapGeometry(void);
};

