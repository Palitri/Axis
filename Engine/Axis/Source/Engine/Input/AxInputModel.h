#pragma once

#include "..\..\AxGlobals.h"

#include "..\Properties\AxProperty.h"

#include "..\Input\AxInput.h"

class AXDLLCLASS AxInputModel
{
public:
	static void ParseInputNames(AxInput *input, AxList<AxProperty*> &properties, const AxString &propertyNames);
	static float ProcessInputProperties(AxList<AxProperty*> &inputProperties, float deltaTime);

	AxInputModel(void);
	virtual ~AxInputModel(void);

	virtual void Process() = 0;
};

