#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxInputModel.h"

#include "..\..\Axis.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxProperty*>;

class AXDLLCLASS AxWalkInputModel :
	public AxInputModel
{
private:
	AxVector3 moveVector;
	AxVector2 turnVector;

public:
	Axis *context;
	AxTransform *transform;

	AxList<AxProperty*> forwardInput, backwardInput, leftInput, rightInput, upInput, downInput, moveConditionInput, turnUpInput, turnDownInput, turnLeftInput, turnRightInput, turnConditionInput;
	float moveSpeed, turnSpeed, moveResponsiveness, turnResponsiveness;

	AxWalkInputModel(Axis *context, AxTransform *transform, float moveSpeed, float moveResponsiveness, float turnSpeed, float turnResponsiveness, const AxString &forwardInput, const AxString &backwardInput, const AxString &leftInput, const AxString &rightInput, const AxString &upInput, const AxString &downInput, const AxString &moveConditionInput, const AxString &turnUpInput, const AxString &turnDownInput, const AxString &turnLeftInput, const AxString &turnRightInput, const AxString &turnConditionInput);
	virtual ~AxWalkInputModel(void);

	virtual void Process();
};

