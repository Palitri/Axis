#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionWhile :
	public AxScriptExpression
{
public:
	AxScriptExpressionWhile(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionWhile(void);

	bool Run();
};

