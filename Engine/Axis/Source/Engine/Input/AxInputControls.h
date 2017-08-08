#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxList.h"

#include "..\Properties\AxProperty.h"

class AXDLLCLASS AxInputControls
	: public AxList<AxProperty*>
{
public:
	float GetValue(float deltaTime);

	AxInputControls(void);
	~AxInputControls(void);
};

