#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionVariable :
	public AxScriptExpression

{
public:
	bool assignmentMode;

	AxScriptExpressionVariable(AxCodeBuilder *codeBuilder, bool assignmentMode);
	~AxScriptExpressionVariable(void);

	bool Run();
};

