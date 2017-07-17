#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionConstantValue :
	public AxScriptExpression

{
public:
	AxScriptExpressionConstantValue(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionConstantValue(void);

	bool Run();
};

