//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOrientationMechanism.h"

#include "..\..\..\Tools\AxMath.h"

#include "..\..\Primitive\AxMatrix.h"
#include "..\..\Primitive\AxVector3.h"

#include "..\..\Entities\Transform\AxTransform.h"

AxOrientationMechanism::AxOrientationMechanism(void)
	: AxMechanism()
{
	this->typeId = AxOrientationMechanism::classId;

	this->parameter.SetType(AxParameterType_None);

	this->properties.Add(new AxProperty("Transform", 0, AxParameterType_ReferenceTransform));
    this->properties.Add(new AxProperty("Target Transform", 0, AxParameterType_ReferenceTransform));
	this->properties.Add(new AxProperty("Inertness", 0.1f));
    this->properties.Add(new AxProperty("Min Angle", 0.1f));
}


AxOrientationMechanism::~AxOrientationMechanism(void)
{
}

bool AxOrientationMechanism::Process(float deltaTime)
{
	AxVector3 currentPosition, targetPosition, targetVector,
			  spherical, newSpherical;
    
	AxMatrix azimuth, elevation;

	AxTransform 
		*transform = (AxTransform*)this->properties[AxOrientationMechanism::propertyIndex_Transform]->value,
		*target = (AxTransform*)this->properties[AxOrientationMechanism::propertyIndex_TargetTransform]->value;
	
	if ((transform == 0) || (target == 0))
		return true;

	float inertness = *(float*)this->properties[AxOrientationMechanism::propertyIndex_Inertness]->value;
	float minAngle = *(float*)this->properties[propertyIndex_MinAngle]->value;

	AxMatrix::GetTranslation(currentPosition, transform->worldMatrix);
	AxMatrix::GetTranslation(targetPosition, target->worldMatrix);
    AxVector3::Subtract(targetVector, currentPosition, targetPosition);
    AxVector3::CartesianToSpherical(spherical, targetVector);
    if (inertness == 0)
        AxVector3::Copy(newSpherical, spherical);
    else
    {
        float transitionLength = AxMath::Sqrt(spherical.x * spherical.x + spherical.y * spherical.y);
        if (transitionLength < minAngle)
            AxVector3::LerpSherical(newSpherical, lastSpherical, spherical, AxMath::Min(deltaTime / inertness, 1.0f));
        else
            // TODO: What is this "else" for?
			// Well, it seems to be here for, yet unimplemented, handling maximum distance
			AxVector3::LerpSherical(newSpherical, lastSpherical, spherical, AxMath::Min(deltaTime / inertness, 1.0f));
    }

    AxVector3::Copy(lastSpherical, newSpherical);

    AxMatrix::CreateRotationZ(azimuth, lastSpherical.y);
    AxMatrix::CreateRotationY(elevation, -lastSpherical.x);
    AxMatrix::Multiply(azimuth, azimuth, elevation);
	AxMatrix::Multiply(transform->worldMatrix, azimuth, transform->worldMatrix);

    return true;
}