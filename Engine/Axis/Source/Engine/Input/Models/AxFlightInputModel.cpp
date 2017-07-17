//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFlightInputModel.h"


AxFlightInputModel::AxFlightInputModel(Axis *context, AxTransform *transform, float moveSpeed, float moveResponsiveness, float turnSpeed, float turnResponsiveness, const AxString &forwardInput, const AxString &backwardInput, const AxString &leftInput, const AxString &rightInput, const AxString &upInput, const AxString &downInput, const AxString &moveConditionInput, const AxString &pitchUpInput, const AxString &pitchDownInput, const AxString &yawLeftInput, const AxString &yawRightInput, const AxString &rollLeftInput, const AxString &rollRightInput, const AxString &turnConditionInput)
{
	this->context = context;
	this->transform = transform;

	AxInputModel::ParseInputNames(this->context->input, this->forwardInput, forwardInput);
	AxInputModel::ParseInputNames(this->context->input, this->backwardInput, backwardInput);
	AxInputModel::ParseInputNames(this->context->input, this->leftInput, leftInput);
	AxInputModel::ParseInputNames(this->context->input, this->rightInput, rightInput);
	AxInputModel::ParseInputNames(this->context->input, this->upInput, upInput);
	AxInputModel::ParseInputNames(this->context->input, this->downInput, downInput);
	AxInputModel::ParseInputNames(this->context->input, this->moveConditionInput, moveConditionInput);
	AxInputModel::ParseInputNames(this->context->input, this->pitchUpInput, pitchUpInput);
	AxInputModel::ParseInputNames(this->context->input, this->pitchDownInput, pitchDownInput);
	AxInputModel::ParseInputNames(this->context->input, this->yawLeftInput, yawLeftInput);
	AxInputModel::ParseInputNames(this->context->input, this->yawRightInput, yawRightInput);
	AxInputModel::ParseInputNames(this->context->input, this->rollLeftInput, rollLeftInput);
	AxInputModel::ParseInputNames(this->context->input, this->rollRightInput, rollRightInput);
	AxInputModel::ParseInputNames(this->context->input, this->turnConditionInput, turnConditionInput);

	this->moveSpeed = moveSpeed;
	this->moveResponsiveness = moveResponsiveness;
	this->turnSpeed = turnSpeed;
	this->turnResponsiveness = turnResponsiveness;
}


AxFlightInputModel::~AxFlightInputModel(void)
{
}

void AxFlightInputModel::Process()
{
	if (this->transform == 0)
		return;

	float speedFactor = this->context->timer->actualTime;

	AxVector3 moveVector;
	if ((this->moveConditionInput.count == 0) || AxInputModel::ProcessInputProperties(this->moveConditionInput, speedFactor) != 0.0f)
	{
		moveVector.x = AxInputModel::ProcessInputProperties(this->leftInput, speedFactor) - AxInputModel::ProcessInputProperties(this->rightInput, speedFactor);
		moveVector.y = AxInputModel::ProcessInputProperties(this->upInput, speedFactor) - AxInputModel::ProcessInputProperties(this->downInput, speedFactor);
		moveVector.z = AxInputModel::ProcessInputProperties(this->forwardInput, speedFactor) - AxInputModel::ProcessInputProperties(this->backwardInput, speedFactor);
	}
	else
		moveVector.Set(0.0f);

	AxVector3::Lerp(this->moveVector, this->moveVector, moveVector, AxMath::Trim(this->moveResponsiveness * speedFactor, 0.0f, 1.0f));
	
	this->transform->TranslateIntrinsic(this->moveVector.Scale(this->moveSpeed));


	AxVector3 turnVector;
	if ((this->turnConditionInput.count == 0) || AxInputModel::ProcessInputProperties(this->turnConditionInput, speedFactor) != 0.0f)
	{
		turnVector.x = AxInputModel::ProcessInputProperties(this->pitchUpInput, speedFactor) - AxInputModel::ProcessInputProperties(this->pitchDownInput, speedFactor);
		turnVector.y = AxInputModel::ProcessInputProperties(this->yawLeftInput, speedFactor) - AxInputModel::ProcessInputProperties(this->yawRightInput, speedFactor);
		turnVector.z = AxInputModel::ProcessInputProperties(this->rollLeftInput, speedFactor) - AxInputModel::ProcessInputProperties(this->rollRightInput, speedFactor);
	}
	else
		turnVector.Set(0.0f);

	AxVector3::Lerp(this->turnVector, this->turnVector, turnVector, AxMath::Trim(this->turnResponsiveness * speedFactor, 0.0f, 1.0f));
	
	float turnSpeedFactor = this->turnSpeed * AxMath::Pi2;
	this->transform->RotateIntrinsic(AxVector3(this->turnVector.x * turnSpeedFactor, 0.0f, 0.0f));
	this->transform->RotateIntrinsic(AxVector3(0.0f, -this->turnVector.y * turnSpeedFactor, 0.0f));
	this->transform->RotateIntrinsic(AxVector3(0.0f, 0.0f, this->turnVector.z * turnSpeedFactor));
}