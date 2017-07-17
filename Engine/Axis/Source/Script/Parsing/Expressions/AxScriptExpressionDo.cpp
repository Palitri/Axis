//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionDo.h"

#include "AxScriptExpressionUnion.h"
#include "AxScriptExpressionEvaluation.h"

AxScriptExpressionDo::AxScriptExpressionDo(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionDo::~AxScriptExpressionDo(void)
{
}

bool AxScriptExpressionDo::Run()
{
	// do
	if (!this->codeBuilder->GetToken()->name.Equals("do"))
		return false;
	this->codeBuilder->tokenIndex++;

	long long jumpAddressDo = this->codeBuilder->code->position;

	// { ... }
	if (!AxScriptExpressionUnion(this->codeBuilder).Run())
		return false;

	// while
	if (!this->codeBuilder->GetToken()->name.Equals("while"))
		return false;
	this->codeBuilder->tokenIndex++;

	// (
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_LeftBracket)
		return false;
	this->codeBuilder->LockTokenRange(this->codeBuilder->tokens.FindClosingBracket(this->codeBuilder->tokenId_RightBracket, this->codeBuilder->tokenIndex) - 1);
	this->codeBuilder->tokenIndex++;

	// x < y
	if (!AxScriptExpressionEvaluation(this->codeBuilder).Run())
		return false;
	this->codeBuilder->tokenIndex--;
	this->codeBuilder->UnlockTokenRange();

	// )
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightBracket)
		return false;
	this->codeBuilder->tokenIndex++;

	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JNZ);
	this->codeBuilder->code->WriteInt32((int)(jumpAddressDo - this->codeBuilder->code->position - 4));

	return true;
}