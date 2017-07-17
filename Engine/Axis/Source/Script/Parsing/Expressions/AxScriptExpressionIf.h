#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionIf :
	public AxScriptExpression
{
public:
	AxScriptExpressionIf(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionIf(void);

	bool Run();
};

