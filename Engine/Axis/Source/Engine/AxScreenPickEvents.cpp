//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScreenPickEvents.h"

#include "Axis.h"

#include "Utilities\AxMaths.h"

AxScreenPickEvents::AxScreenPickEvents(Axis &Axis)
	: AxTraceEvents(Axis)
{
}

AxScreenPickEvents::~AxScreenPickEvents(void)
{
}

void AxScreenPickEvents::SetupScreenPick(AxVector2 &screenCoords)
{
    this->screenCoords.Set(screenCoords, 0.0f);
	this->intersectionInfo.hasIntersected = false;
	this->intersectionInfo.distance = AxMath::FloatMax;
}
	
void AxScreenPickEvents::OnMesh(AxTraceParameters &parameters)
{
	if ((parameters.cameraRef == 0) || (parameters.transformRef == 0))
		return;

	AxMatrix transform;
	AxMatrix::Multiply(transform, parameters.transformRef->pivotedWorldMatrix, parameters.cameraRef->viewProjection);
	if (!parameters.meshRef->bounds.InsideViewFrustum(transform))
		return;

	AxVector3 origin, direction;
	AxMaths::ScreenSpaceToVolumetricRay(origin, direction, this->screenCoords, transform);

	AxIntersectionInfo intersectionInfo;
    if (parameters.meshRef->Intersection(origin, direction, intersectionInfo))
	{
		AxVector3 cameraSpacePosition;
		AxVector3::Transform(cameraSpacePosition, intersectionInfo.point.position, transform);
		intersectionInfo.distance = AxVector3::Distance(this->screenCoords, cameraSpacePosition);

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