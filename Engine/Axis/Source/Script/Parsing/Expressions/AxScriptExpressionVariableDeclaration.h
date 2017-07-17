#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionVariableDeclaration :
	public AxScriptExpression
{
public:
	AxScriptExpressionVariableDeclaration(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionVariableDeclaration(void);

	bool Run();
};

