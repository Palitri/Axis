#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionFor :
	public AxScriptExpression
{
public:
	AxScriptExpressionFor(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionFor(void);

	bool Run();
};

