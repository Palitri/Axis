//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxRotationInputModel.h"


AxRotationInputModel::AxRotationInputModel(Axis *context, AxTransform *transform, float rotationSpeed, float rotationResponsiveness, const AxString &upInput, const AxString &downInput, const AxString &leftInput, const AxString &rightInput, const AxString &rollLeftInput, const AxString &rollRightInput, const AxString &rotationConditionInput)
{
	this->context = context;
	this->transform = transform;
	
	this->rotationSpeed = rotationSpeed;
	this->rotationResponsiveness = rotationResponsiveness;

	this->context->input->GetInputControls(this->upInput, upInput);
	this->context->input->GetInputControls(this->downInput, downInput);
	this->context->input->GetInputControls(this->leftInput, leftInput);
	this->context->input->GetInputControls(this->rightInput, rightInput);
	this->context->input->GetInputControls(this->rollLeftInput, rollLeftInput);
	this->context->input->GetInputControls(this->rollRightInput, rollRightInput);
	this->context->input->GetInputControls(this->rotationConditionInput, rotationConditionInput);
}


AxRotationInputModel::~AxRotationInputModel(void)
{
}

void AxRotationInputModel::Process()
{
	if (this->transform == 0)
		return;

	float speedFactor = this->context->timer->actualTime;

	AxVector3 rotationVector;
	if ((this->rotationConditionInput.count == 0) || this->rotationConditionInput.GetValue(speedFactor) != 0.0f)
	{
		rotationVector.x = this->leftInput.GetValue(speedFactor) - this->rightInput.GetValue(speedFactor);
		rotationVector.y = this->upInput.GetValue(speedFactor) - this->downInput.GetValue(speedFactor);
		rotationVector.z = this->rollLeftInput.GetValue(speedFactor) - this->rollRightInput.GetValue(speedFactor);
	}
	else
		rotationVector.Set(0.0f);

	AxVector3::Lerp(this->rotationVector, this->rotationVector, rotationVector, AxMath::Trim(this->rotationResponsiveness * speedFactor, 0.0f, 1.0f));
	
	float rotationSpeedFactor = this->rotationSpeed * AxMath::Pi2;
	if (this->context->currentCamera.transformRef == 0)
	{
		this->transform->RotateIntrinsic(AxVector3(0.0f, this->rotationVector.x * rotationSpeedFactor, 0.0f));
		this->transform->RotateExtrinsic(AxVector3(this->rotationVector.y * rotationSpeedFactor, 0.0f, 0.0f));
		this->transform->RotateExtrinsic(AxVector3(0.0f, 0.0f, this->rotationVector.z * rotationSpeedFactor));
	}
	else
	{
		this->transform->RotateIntrinsic(AxVector3(0.0f, this->rotationVector.x * rotationSpeedFactor, 0.0f));
		this->transform->RotateAroundExtrinsic(this->context->currentCamera.transformRef->GetRightVectorExtrinsic(), -this->rotationVector.y * rotationSpeedFactor);
		this->transform->RotateAroundExtrinsic(this->context->currentCamera.transformRef->GetForwardVectorExtrinsic(), this->rotationVector.z * rotationSpeedFactor);
	}
}
