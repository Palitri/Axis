#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionCallout :
	public AxScriptExpression
{
public:
	AxScriptExpressionCallout(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionCallout(void);

	bool Run();
};

