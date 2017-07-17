#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxMath.h"

#include "..\..\Base\AxResource.h"

#include "..\..\Primitive\AxMatrix.h"
#include "..\..\Primitive\AxVector3.h"

class Axis;

class AXDLLCLASS AxCamera
	: public AxResource
{
public:
	static const int classId = (AxResourceType_Camera << 16) | 0;

	static const int propertyIndex_Aspect		= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_FOV			= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_OrthoZoom	= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Orthographic	= AxResource::propertyIndex_ChildPropertiesIndex + 3;

	static const int FOVMode_Vertical = 0;
	static const int FOVMode_Horizontal = 1;
	static const int FOVMode_LesserScreenDimension = 2;
	static const int FOVMode_GreaterScreenDimension = 3;

	Axis *context;

	AxMatrix view, projection, viewProjection;
    AxVector3 position, lookAt, xAxis, yAxis, zAxis;

	AxCamera(Axis *context);
	virtual ~AxCamera(void);

    void BuildMatrix(AxMatrix &transform);

	AxVector3 CastVector(AxVector2 &screenCoords);
	AxVector3 ProjectVector(AxVector3 vector);
};

