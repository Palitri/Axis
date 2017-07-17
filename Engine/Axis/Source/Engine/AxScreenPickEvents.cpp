//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScreenPickEvents.h"

#include "Axis.h"

AxScreenPickEvents::AxScreenPickEvents(Axis &Axis)
	: AxTraceEvents(Axis)
{
}

AxScreenPickEvents::~AxScreenPickEvents(void)
{
}

void AxScreenPickEvents::SetupScreenPick(AxVector2 &screenCoords)
{
    this->rayPoint1 = AxVector3(screenCoords, -1.0f);
    this->rayPoint2 = AxVector3(screenCoords, 1.0f);
	this->intersectionInfo.hasIntersected = false;
	this->intersectionInfo.distance = AxMath::FloatMax;
}
	
void AxScreenPickEvents::OnMesh(AxTraceParameters &parameters)
{
	AxMatrix transform, invertTransform;
	AxVector4 origin4, direction4;
	AxVector3 origin, direction;
	AxIntersectionInfo intersectionInfo;

	if ((parameters.cameraRef == 0) || (parameters.transformRef == 0))
		return;

	AxMatrix::Multiply(transform, parameters.transformRef->pivotedWorldMatrix, parameters.cameraRef->viewProjection);
    
	if (!parameters.meshRef->bounds.InsideViewFrustum(transform))
        return;

	AxMatrix::Invert(invertTransform, transform);
    AxVector4::Transform(origin4, this->rayPoint1, invertTransform);
    AxVector4::Transform(direction4, this->rayPoint2, invertTransform);
    origin.x = origin4.x / origin4.w; 
    origin.y = origin4.y / origin4.w; 
    origin.z = origin4.z / origin4.w;
    direction.x = direction4.x / direction4.w;
    direction.y = direction4.y / direction4.w;
    direction.z = direction4.z / direction4.w;
    AxVector3::Subtract(direction, direction, origin);
    AxVector3::Normalize(direction, direction);
    if (parameters.meshRef->Intersection(origin, direction, intersectionInfo))
	{
		AxVector3 cameraSpacePosition;
		AxVector3::Transform(cameraSpacePosition, intersectionInfo.point.position, transform);
		intersectionInfo.distance = AxVector3::Distance(this->rayPoint1, cameraSpacePosition);

        if (intersectionInfo.distance < this->intersectionInfo.distance)
        {
            this->entityInfo = parameters;
            this->intersectionInfo = intersectionInfo;
        }
	}
}

void AxScreenPickEvents::OnTransform(AxTraceParameters &parameters)
{
	parameters.transformRef->Process(parameters.parentTransformRef);
}