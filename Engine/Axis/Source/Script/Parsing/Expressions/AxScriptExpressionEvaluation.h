#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxScriptExpression.h"

class AxScriptExpressionEvaluation :
	public AxScriptExpression

{
private:
	bool EvaluateExpression(int startToken, int endToken);

public:
	// endTokens are considered part of the expression and after evaluation, the token pointer is set beyond an endToken
	// terminationTokens are not considered part of the expression and after evaluation, the token pointer is set to the termination token, which is just after the expression
	AxList<int> endTokens, terminationTokens;

	AxScriptExpressionEvaluation(AxCodeBuilder *codeBuilder);
	~AxScriptExpressionEvaluation(void);

	bool Run();
};

