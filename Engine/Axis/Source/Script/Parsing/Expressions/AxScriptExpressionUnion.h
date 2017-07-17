#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionUnion :
	public AxScriptExpression
{
public:
	AxScriptExpressionUnion(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionUnion(void);

	bool Run();
};

