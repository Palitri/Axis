#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionFunctionDeclaration :
	public AxScriptExpression
{
public:
	AxScriptExpressionFunctionDeclaration(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionFunctionDeclaration(void);

	bool Run();
};

