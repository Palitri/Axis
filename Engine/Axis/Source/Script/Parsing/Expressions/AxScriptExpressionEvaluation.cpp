//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionEvaluation.h"

#include "AxScriptExpressionConstantValue.h"
#include "AxScriptExpressionVariable.h"

AxScriptExpressionEvaluation::AxScriptExpressionEvaluation(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
	this->endTokens.Add(this->codeBuilder->tokenId_SemiColon);
	this->endTokens.Add(this->codeBuilder->tokenId_Comma);

	this->terminationTokens.Add(this->codeBuilder->tokenId_RightBracket);
}


AxScriptExpressionEvaluation::~AxScriptExpressionEvaluation(void)
{
}

bool AxScriptExpressionEvaluation::EvaluateExpression(int startToken, int endToken)
{
	if (this->codeBuilder->tokens[startToken].id == this->codeBuilder->tokenId_LeftBracket)
    {
        // if we have a fully-bracketed expression, reevaluate it without the brackets.
        if (this->codeBuilder->tokens.FindClosingBracket(this->codeBuilder->tokenId_RightBracket, startToken) == endToken)
            return this->EvaluateExpression(startToken + 1, endToken - 1);
    }

	int operationId;
	int operationTokenId = this->codeBuilder->FindTokenEvaluationOperationByPriority(startToken, endToken, operationId);

    if (operationTokenId == -1)
    {
		this->codeBuilder->tokenIndex = startToken;
		if (!AxScriptExpressionConstantValue(this->codeBuilder).Build())
			if (!AxScriptExpressionVariable(this->codeBuilder, false).Build())
				return false;

		return this->codeBuilder->tokenIndex > endToken;
    }
    else
    {
        bool right = this->EvaluateExpression(operationTokenId + 1, endToken);
        bool left = this->EvaluateExpression(startToken, operationTokenId - 1);

		this->codeBuilder->code->WriteUInt8(operationId);

        return left && right;
    }
}

bool AxScriptExpressionEvaluation::Run()
{
	int start = this->codeBuilder->tokenIndex;
	int end = 0;

	bool ended = false;
	bool terminated = false;
	while (this->codeBuilder->GetToken(end) != this->codeBuilder->invalidToken)
	{
		int tokenId = this->codeBuilder->GetToken(end)->id;
		ended = this->endTokens.Contains(tokenId);
		terminated = this->terminationTokens.Contains(tokenId);
		if (ended || terminated)
			break;
		else
			end++;
	}
	end += start;

	bool result = this->EvaluateExpression(start, end - 1);

	if (terminated)
		this->codeBuilder->tokenIndex = end;
	else
		this->codeBuilder->tokenIndex = end + 1;

	return result;
}
