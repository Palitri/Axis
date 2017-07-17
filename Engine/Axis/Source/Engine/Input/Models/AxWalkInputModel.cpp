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

	AxInputModel::ParseInputNames(this->context->input, this->forwardInput, forwardInput);
	AxInputModel::ParseInputNames(this->context->input, this->backwardInput, backwardInput);
	AxInputModel::ParseInputNames(this->context->input, this->leftInput, leftInput);
	AxInputModel::ParseInputNames(this->context->input, this->rightInput, rightInput);
	AxInputModel::ParseInputNames(this->context->input, this->upInput, upInput);
	AxInputModel::ParseInputNames(this->context->input, this->downInput, downInput);
	AxInputModel::ParseInputNames(this->context->input, this->moveConditionInput, moveConditionInput);
	AxInputModel::ParseInputNames(this->context->input, this->turnUpInput, turnUpInput);
	AxInputModel::ParseInputNames(this->context->input, this->turnDownInput, turnDownInput);
	AxInputModel::ParseInputNames(this->context->input, this->turnLeftInput, turnLeftInput);
	AxInputModel::ParseInputNames(this->context->input, this->turnRightInput, turnRightInput);
	AxInputModel::ParseInputNames(this->context->input, this->turnConditionInput, turnConditionInput);

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


	AxVector2 turnVector;
	if ((this->turnConditionInput.count == 0) || AxInputModel::ProcessInputProperties(this->turnConditionInput, speedFactor) != 0.0f)
	{
		turnVector.x = AxInputModel::ProcessInputProperties(this->turnUpInput, speedFactor) - AxInputModel::ProcessInputProperties(this->turnDownInput, speedFactor);
		turnVector.y = AxInputModel::ProcessInputProperties(this->turnLeftInput, speedFactor) - AxInputModel::ProcessInputProperties(this->turnRightInput, speedFactor);
	}
	else
		turnVector.Set(0.0f);

	AxVector2::Lerp(this->turnVector, this->turnVector, turnVector, AxMath::Trim(this->turnResponsiveness * speedFactor, 0.0f, 1.0f));
	
	float turnSpeedFactor = this->turnSpeed * AxMath::Pi2;
	this->transform->RotateExtrinsic(AxVector3(0.0f, this->turnVector.y * turnSpeedFactor, 0.0f));
	this->transform->RotateIntrinsic(AxVector3(this->turnVector.x * turnSpeedFactor, 0.0f, 0.0f));
}