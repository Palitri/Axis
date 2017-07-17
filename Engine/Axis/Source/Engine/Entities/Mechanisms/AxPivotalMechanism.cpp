//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPivotalMechanism.h"

#include "..\..\Primitive\AxVector3.h"

#include "..\..\Entities\Transform\AxTransform.h"

AxPivotalMechanism::AxPivotalMechanism(void)
	: AxMechanism()
{
	this->typeId = AxPivotalMechanism::classId;

	this->parameter.SetType(AxParameterType_None);

    this->properties.Add(new AxProperty("Transform", 0, AxParameterType_ReferenceTransform));
    this->properties.Add(new AxProperty("Target Transform", 0, AxParameterType_ReferenceTransform));

	this->properties.Add(new AxProperty("Environment Resistance", 0.5f));
    this->properties.Add(new AxProperty("Gravity Strength", 0.0f));

    this->properties.Add(new AxProperty("Connection Length", 50.0f));
    this->properties.Add(new AxProperty("Elasticity Length", 100.0f));
    this->properties.Add(new AxProperty("Connection Solid", false));

    this->properties.Add(new AxProperty("Target Mass", 5.0f));

	this->gravityVector = AxVector3(0.0f, -1.0f, 0.0f);
}


AxPivotalMechanism::~AxPivotalMechanism(void)
{
}


bool AxPivotalMechanism::Process(float deltaTime)
{
	AxVector3 pivot, moveVector, newTargetPos;

	AxTransform 
		*transform = (AxTransform*)properties[AxPivotalMechanism::propertyIndex_Transform]->value,
		*target = (AxTransform*)properties[AxPivotalMechanism::propertyIndex_TargetTransform]->value;

	if ((transform == 0) || (target == 0))
		return true;

	float 
		environmentResistance = *(float*)properties[AxPivotalMechanism::propertyIndex_EnvironmentResistance]->value,
        gravityStrength = *(float*)properties[AxPivotalMechanism::propertyIndex_GravityStrength]->value,
		connectionLength = *(float*)properties[AxPivotalMechanism::propertyIndex_ConnectionLength]->value,
        elasticityLength = *(float*)properties[AxPivotalMechanism::propertyIndex_ElasticityLength]->value;
	bool connectionSolid = *(bool*)properties[AxPivotalMechanism::propertyIndex_ConnectionSolid]->value;
	float targetMass = *(float*)properties[AxPivotalMechanism::propertyIndex_TargetMass]->value;

	AxMatrix::GetTranslation(pivot, transform->worldMatrix);
    AxVector3::Add(newTargetPos, this->targetPos, this->targetMoveVector);
    AxVector3::Subtract(moveVector, pivot, newTargetPos);
    float distance = moveVector.GetLength();
        
    float vectorLength = distance - connectionLength;

    if (distance > 0.0f)
        AxVector3::Scale(moveVector, moveVector, 1.0f / distance);

    if ((vectorLength > 0.0f) || connectionSolid)
    {
        AxVector3::Scale(moveVector, moveVector, vectorLength / elasticityLength);
        AxVector3::Add(targetMoveVector, targetMoveVector, moveVector);
    }

    AxVector3::Scale(gravityVector, gravityVector, gravityStrength);
    AxVector3::Add(targetMoveVector, targetMoveVector, gravityVector);

    AxVector3::Scale(targetMoveVector, targetMoveVector, 1.0f - environmentResistance / targetMass);

    AxVector3::Add(targetPos, targetPos, targetMoveVector);

	AxMatrix::SetTranslation(target->worldMatrix, targetPos);

    return true;
}