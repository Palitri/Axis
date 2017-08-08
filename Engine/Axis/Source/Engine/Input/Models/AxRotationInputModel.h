#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxInputModel.h"
#include "..\AxInputControls.h"

#include "..\..\Axis.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxProperty*>;

class AXDLLCLASS AxRotationInputModel :
	public AxInputModel
{
private:
	AxVector3 rotationVector;

public:
	Axis *context;
	AxTransform *transform;

	float rotationSpeed, rotationResponsiveness;
	AxInputControls upInput, downInput, leftInput, rightInput, rollLeftInput, rollRightInput, rotationConditionInput;

	AxRotationInputModel(Axis *context, AxTransform *transform, float rotationSpeed, float rotationResponsiveness, const AxString &upInput, const AxString &downInput, const AxString &leftInput, const AxString &rightInput, const AxString &rollLeftInput, const AxString &rollRightInput, const AxString &rotationConditionInput);
	virtual ~AxRotationInputModel(void);

	virtual void Process();
};

