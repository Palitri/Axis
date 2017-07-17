#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\AxString.h"
#include "..\AxWebRequest.h"

class AxWebServer;

class AXDLLCLASS AxWebPageComponent
{
public:
	AxWebServer *webServer;
	AxString name;

	AxWebPageComponent(void);
	virtual ~AxWebPageComponent(void);

	virtual AxString Output(AxWebRequest *request) = 0;
};

