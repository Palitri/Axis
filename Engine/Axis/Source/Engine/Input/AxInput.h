#pragma once

#include "..\..\AxGlobals.h"

#include "..\Base\AxResource.h"

#include "AxInputControls.h"

class AXDLLCLASS AxInput :
	public AxResource
{
public:
	AxInput(void);
	virtual ~AxInput(void);

	virtual AxProperty *AcquireInputControl(const AxString &inputName, AxParameterType inputType);
	
	virtual void GetInputControls(AxInputControls &result, const AxString &inputNames);
};
