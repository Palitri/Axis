//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOrbitInputModel.h"


AxOrbitInputModel::AxOrbitInputModel(Axis *context, AxTransform *pivotalTransform, AxTransform *orbitalTransform, float rotationSpeed, float rotationResponsiveness, const AxString &upInput, const AxString &downInput, const AxString &leftInput, const AxString &rightInput, const AxString &rollLeftInput, const AxString &rollRightInput, const AxString &orbitConditionInput)
{
	this->context = context;
	this->pivotalTransform = pivotalTransform;
	this->orbitalTransform = orbitalTransform;
	
	this->rotationSpeed = rotationSpeed;
	this->rotationResponsiveness = rotationResponsiveness;

	AxInputModel::ParseInputNames(this->context->input, this->upInput, upInput);
	AxInputModel::ParseInputNames(this->context->input, this->downInput, downInput);
	AxInputModel::ParseInputNames(this->context->input, this->leftInput, leftInput);
	AxInputModel::ParseInputNames(this->context->input, this->rightInput, rightInput);
	AxInputModel::ParseInputNames(this->context->input, this->rollLeftInput, rollLeftInput);
	AxInputModel::ParseInputNames(this->context->input, this->rollRightInput, rollRightInput);
	AxInputModel::ParseInputNames(this->context->input, this->orbitConditionInput, orbitConditionInput);
}


AxOrbitInputModel::~AxOrbitInputModel(void)
{
}

void AxOrbitInputModel::Process()
{
	AxTransform *orbitalTransform = this->orbitalTransform != 0 ? this->orbitalTransform : this->context->currentCamera.transformRef;

	if ((this->pivotalTransform == 0) || (orbitalTransform == 0))
		return;

	float speedFactor = this->context->timer->actualTime;

	AxVector3 orbitVector;
	if ((this->orbitConditionInput.count == 0) || AxInputModel::ProcessInputProperties(this->orbitConditionInput, speedFactor) != 0.0f)
	{
		orbitVector.x = AxInputModel::ProcessInputProperties(this->leftInput, speedFactor) - AxInputModel::ProcessInputProperties(this->rightInput, speedFactor);
		orbitVector.y = AxInputModel::ProcessInputProperties(this->upInput, speedFactor) - AxInputModel::ProcessInputProperties(this->downInput, speedFactor);
		orbitVector.z = AxInputModel::ProcessInputProperties(this->rollLeftInput, speedFactor) - AxInputModel::ProcessInputProperties(this->rollRightInput, speedFactor);
	}
	else
		orbitVector.Set(0.0f);

	AxVector3::Lerp(this->orbitVector, this->orbitVector, orbitVector, AxMath::Trim(this->rotationResponsiveness * speedFactor, 0.0f, 1.0f));
	
	float rotationSpeedFactor = this->rotationSpeed * AxMath::Pi2;
	AxVector3 rayStart = this->pivotalTransform->GetPositionExtrinsic();
	orbitalTransform->RotateAroundRayExtrinsic(rayStart, AxVector3(0.0f, 1.0f, 0.0f), this->orbitVector.x * rotationSpeedFactor);
	orbitalTransform->RotateAroundRayExtrinsic(rayStart, orbitalTransform->GetRightVectorExtrinsic(), this->orbitVector.y * rotationSpeedFactor);
	orbitalTransform->RotateIntrinsic(AxVector3(0.0f, 0.0f, this->orbitVector.z * rotationSpeedFactor));
}
