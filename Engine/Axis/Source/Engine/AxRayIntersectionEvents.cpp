//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxRayIntersectionEvents.h"


AxRayIntersectionEvents::AxRayIntersectionEvents(Axis &context)
	: AxTraceEvents(context)
{
}


AxRayIntersectionEvents::~AxRayIntersectionEvents(void)
{
}

void AxRayIntersectionEvents::SetupRay(AxVector3 &rayInitialPoint, AxVector3 &rayOrientation)
{
    AxVector3::Copy(this->rayPoint, rayInitialPoint);
    AxVector3::Normalize(this->rayVector, rayOrientation);
	this->intersectionDistance = AxMath::FloatMax;
    this->intersected = false;
}
    
void AxRayIntersectionEvents::OnMesh(AxTraceParameters &parameters)
{
	if (parameters.transformRef == 0)
		return;

	AxMatrix transform;
	AxVector3 origin, direction;
    AxIntersectionInfo intersectionInfo;

	AxMatrix::Invert(transform, parameters.transformRef->pivotedWorldMatrix);
    AxVector3::Transform(origin, this->rayPoint, transform);
    AxVector3::TransformNormal(direction, this->rayVector, transform);
    if (parameters.meshRef->Intersection(origin, direction, intersectionInfo))
	{
        if (intersectionInfo.distance < this->intersectionDistance)
        {
            this->traceResult = parameters;
            this->intersectionInfo = intersectionInfo;
            this->intersectionDistance = intersectionInfo.distance;
            this->intersected = true;
        }
	}
}

void AxRayIntersectionEvents::OnTransform(AxTraceParameters &parameters)
{
	parameters.transformRef->Process(parameters.parentTransformRef);
}