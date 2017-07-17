#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxWebPageComponent.h"

class AXDLLCLASS AxWebPageFileListing
	: public AxWebPageComponent
{
public:
	AxWebPageFileListing(void);
	~AxWebPageFileListing(void);

	AxString Output(AxWebRequest *request);
};

