#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxInputModel.h"

#include "..\..\Axis.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxProperty*>;

class AXDLLCLASS AxFlightInputModel :
	public AxInputModel
{
private:
	AxVector3 moveVector;
	AxVector3 turnVector;

public:
	Axis *context;
	AxTransform *transform;

	AxList<AxProperty*> forwardInput, backwardInput, leftInput, rightInput, upInput, downInput, moveConditionInput, pitchUpInput, pitchDownInput, yawLeftInput, yawRightInput, rollLeftInput, rollRightInput, turnConditionInput;
	float moveSpeed, turnSpeed, moveResponsiveness, turnResponsiveness;

	AxFlightInputModel(Axis *context, AxTransform *transform, float moveSpeed, float moveResponsiveness, float turnSpeed, float turnResponsiveness, const AxString &forwardInput, const AxString &backwardInput, const AxString &leftInput, const AxString &rightInput, const AxString &upInput, const AxString &downInput, const AxString &moveConditionInput, const AxString &pitchUpInput, const AxString &pitchDownInput, const AxString &yawLeftInput, const AxString &yawRightInput, const AxString &rollLeftInput, const AxString &rollRightInput, const AxString &turnConditionInput);
	virtual ~AxFlightInputModel(void);

	virtual void Process();
};

