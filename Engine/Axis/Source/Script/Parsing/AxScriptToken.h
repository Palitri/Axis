#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxString.h"

class AXDLLCLASS AxScriptToken
{
public:
	AxString name;
	int id;
	int start, end;

	AxScriptToken(void);
	AxScriptToken(AxString name, int id, int start, int end);
	~AxScriptToken(void);
};

