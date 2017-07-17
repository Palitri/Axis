#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionAssignment :
	public AxScriptExpression
{
public:
	AxScriptExpressionAssignment(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionAssignment(void);

	bool Run();
};

