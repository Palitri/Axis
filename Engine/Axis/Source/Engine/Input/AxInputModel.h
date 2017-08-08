#pragma once

#include "..\..\AxGlobals.h"

#include "..\Properties\AxProperty.h"

#include "..\Input\AxInput.h"

class AXDLLCLASS AxInputModel
{
public:
	AxInputModel(void);
	virtual ~AxInputModel(void);

	virtual void Process() = 0;
};

