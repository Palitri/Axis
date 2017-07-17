#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionDo :
	public AxScriptExpression
{
public:
	AxScriptExpressionDo(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionDo(void);

	bool Run();
};

