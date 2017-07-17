#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxWebPageComponent.h"

class AXDLLCLASS AxWebPageHostName
	: public AxWebPageComponent
{
public:
	AxWebPageHostName(void);
	~AxWebPageHostName(void);

	AxString Output(AxWebRequest *request);
};

