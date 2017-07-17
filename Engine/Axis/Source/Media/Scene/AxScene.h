#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxMedia.h"

class Axis;

class AXDLLCLASS AxScene :
	public AxMedia
{
protected:
	Axis *context;

public:
	AxScene(void);
	virtual ~AxScene(void);

	void SetContext(Axis *context);
};

