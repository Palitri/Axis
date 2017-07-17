#pragma once

#include "..\..\AxGlobals.h"

#include "..\Base\AxResource.h"

class AXDLLCLASS AxInput :
	public AxResource
{
public:
	AxInput(void);
	virtual ~AxInput(void);

	virtual AxProperty *AcquireInput(const AxString &inputName, AxParameterType inputType);
};
