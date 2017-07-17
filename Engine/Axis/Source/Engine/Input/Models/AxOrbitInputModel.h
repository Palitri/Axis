#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxInputModel.h"

#include "..\..\Axis.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxProperty*>;

class AXDLLCLASS AxOrbitInputModel :
	public AxInputModel
{
private:
	AxVector3 orbitVector;

public:
	Axis *context;
	AxTransform *pivotalTransform, *orbitalTransform;

	float rotationSpeed, rotationResponsiveness;
	AxList<AxProperty*> upInput, downInput, leftInput, rightInput, rollLeftInput, rollRightInput, orbitConditionInput;

	AxOrbitInputModel(Axis *context, AxTransform *pivotalTransform, AxTransform *orbitalTransform, float rotationSpeed, float rotationResponsiveness, const AxString &upInput, const AxString &downInput, const AxString &leftInput, const AxString &rightInput, const AxString &rollLeftInput, const AxString &rollRightInput, const AxString &orbitConditionInput);
	virtual ~AxOrbitInputModel(void);

	virtual void Process();
};

