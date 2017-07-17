#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxCodeBuilder.h"

#include "..\..\Processing\AxCodeProcessor.h"

class AxScriptExpression
{
public:
	AxCodeBuilder *codeBuilder;

	AxScriptExpression(AxCodeBuilder *codeBuilder);
	~AxScriptExpression(void);

	// If succeeds, returns true and leaves the token index after the end of the expression's last token (and at the beginning of the next, if any)
	// If fails, returns false. The token index and other code generation variables remain undefined (typically, around the place of error) and should be reset to the state before
	// This method can be called inside other AxScriptExpression Run methods, but only with having in mind that if it fails, the codeBuilder state (tokenIndex, code position and etc.) remain undefined.
	virtual bool Run() = 0;

	// Executes the Run method and if it fails, resets the codeBuilder's state as it was before the call, so the codeBuilder is ready for any other expression to try to build
	bool Build();
};

