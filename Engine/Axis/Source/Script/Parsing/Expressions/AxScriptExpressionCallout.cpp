//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionCallout.h"

#include "AxScriptExpressionEvaluation.h"
#include "AxScriptExpressionUnion.h"


AxScriptExpressionCallout::AxScriptExpressionCallout(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionCallout::~AxScriptExpressionCallout(void)
{
}

bool AxScriptExpressionCallout::Run()
{
	// Callout
	if (!this->codeBuilder->GetToken()->name.Equals("Callout"))
		return false;
	this->codeBuilder->tokenIndex++;

	// (
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_LeftBracket)
		return false;
	this->codeBuilder->tokenIndex++;

	bool success;
	do
	{
		//AxScriptExpressionEvaluation expression(this->codeBuilder);
		//expression.endTokens.Add(this->codeBuilder->tokenId_Comma);
		//expression.terminationTokens.Add(this->codeBuilder->tokenId_RightBracket);
		//success = expression.Run();
		success = AxScriptExpressionUnion(this->codeBuilder).Run();
	} while ((!success) || (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightBracket));

	if (!success)
		return false;

	// )
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightBracket)
		return false;
	this->codeBuilder->tokenIndex++;

	// ;
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_SemiColon)
		return false;
	this->codeBuilder->tokenIndex++;


	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_CallOut);

	return true;
}