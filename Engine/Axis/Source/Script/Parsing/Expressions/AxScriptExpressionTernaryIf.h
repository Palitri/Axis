#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionTernaryIf :
	public AxScriptExpression
{
public:
	AxScriptExpressionTernaryIf(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionTernaryIf(void);

	bool Run();
};

