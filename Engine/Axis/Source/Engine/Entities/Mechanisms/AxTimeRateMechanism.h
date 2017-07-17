#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

class Axis;

class AXDLLCLASS AxTimeRateMechanism :
	public AxMechanism
{
private:
	Axis *context;

public:
	static const int classId = AxMechanism::classId | 111;

	AxTimeRateMechanism(Axis *context);
	virtual ~AxTimeRateMechanism(void);

	bool Process(float deltaTime);
};

