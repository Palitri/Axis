//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWalkInputModel.h"


AxWalkInputModel::AxWalkInputModel(Axis *context, AxTransform *transform, float moveSpeed, float moveResponsiveness, float turnSpeed, float turnResponsiveness, const AxString &forwardInput, const AxString &backwardInput, const AxString &leftInput, const AxString &rightInput, const AxString &upInput, const AxString &downInput, const AxString &moveConditionInput, const AxString &turnUpInput, const AxString &turnDownInput, const AxString &turnLeftInput, const AxString &turnRightInput, const AxString &turnConditionInput)
{
	this->context = context;
	this->transform = transform;

	this->context->input->GetInputControls(this->forwardInput, forwardInput);
	this->context->input->GetInputControls(this->backwardInput, backwardInput);
	this->context->input->GetInputControls(this->leftInput, leftInput);
	this->context->input->GetInputControls(this->rightInput, rightInput);
	this->context->input->GetInputControls(this->upInput, upInput);
	this->context->input->GetInputControls(this->downInput, downInput);
	this->context->input->GetInputControls(this->moveConditionInput, moveConditionInput);
	this->context->input->GetInputControls(this->turnUpInput, turnUpInput);
	this->context->input->GetInputControls(this->turnDownInput, turnDownInput);
	this->context->input->GetInputControls(this->turnLeftInput, turnLeftInput);
	this->context->input->GetInputControls(this->turnRightInput, turnRightInput);
	this->context->input->GetInputControls(this->turnConditionInput, turnConditionInput);

	this->moveSpeed = moveSpeed;
	this->moveResponsiveness = moveResponsiveness;
	this->turnSpeed = turnSpeed;
	this->turnResponsiveness = turnResponsiveness;
}


AxWalkInputModel::~AxWalkInputModel(void)
{
}

void AxWalkInputModel::Process()
{
	if (this->transform == 0)
		return;

	float speedFactor = this->context->timer->actualTime;

	AxVector3 moveVector;
	if ((this->moveConditionInput.count == 0) || this->moveConditionInput.GetValue(speedFactor) != 0.0f)
	{
		moveVector.x = this->leftInput.GetValue(speedFactor) - this->rightInput.GetValue(speedFactor);
		moveVector.y = this->upInput.GetValue(speedFactor) - this->downInput.GetValue(speedFactor);
		moveVector.z = this->forwardInput.GetValue(speedFactor) - this->backwardInput.GetValue(speedFactor);
	}
	else
		moveVector.Set(0.0f);

	AxVector3::Lerp(this->moveVector, this->moveVector, moveVector, AxMath::Trim(this->moveResponsiveness * speedFactor, 0.0f, 1.0f));
	
	this->transform->TranslateIntrinsic(this->moveVector.Scale(this->moveSpeed));


	AxVector2 turnVector;
	if ((this->turnConditionInput.count == 0) || this->turnConditionInput.GetValue(speedFactor) != 0.0f)
	{
		turnVector.x = this->turnUpInput.GetValue(speedFactor) - this->turnDownInput.GetValue(speedFactor);
		turnVector.y = this->turnLeftInput.GetValue(speedFactor) - this->turnRightInput.GetValue(speedFactor);
	}
	else
		turnVector.Set(0.0f);

	AxVector2::Lerp(this->turnVector, this->turnVector, turnVector, AxMath::Trim(this->turnResponsiveness * speedFactor, 0.0f, 1.0f));
	
	float turnSpeedFactor = this->turnSpeed * AxMath::Pi2;
	this->transform->RotateExtrinsic(AxVector3(0.0f, this->turnVector.y * turnSpeedFactor, 0.0f));
	this->transform->RotateIntrinsic(AxVector3(this->turnVector.x * turnSpeedFactor, 0.0f, 0.0f));
}