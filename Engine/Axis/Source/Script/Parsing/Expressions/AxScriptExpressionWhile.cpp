//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionWhile.h"

#include "AxScriptExpressionEvaluation.h"
#include "AxScriptExpressionUnion.h"

AxScriptExpressionWhile::AxScriptExpressionWhile(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionWhile::~AxScriptExpressionWhile(void)
{
}

bool AxScriptExpressionWhile::Run()
{
	// while
	if (!this->codeBuilder->GetToken()->name.Equals("while"))
		return false;
	this->codeBuilder->tokenIndex++;

	// (
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_LeftBracket)
		return false;
	this->codeBuilder->LockTokenRange(this->codeBuilder->tokens.FindClosingBracket(this->codeBuilder->tokenId_RightBracket, this->codeBuilder->tokenIndex) - 1);
	this->codeBuilder->tokenIndex++;

	long long jumpAddressConditionWhile = this->codeBuilder->code->position;

	// x < y
	if (!AxScriptExpressionEvaluation(this->codeBuilder).Run())
		return false;
	this->codeBuilder->tokenIndex--;

	// )
	this->codeBuilder->UnlockTokenRange();
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightBracket)
		return false;
	this->codeBuilder->tokenIndex++;

	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JZ);
	long long jumpAddressEndWhile = this->codeBuilder->code->position;
	this->codeBuilder->code->WriteInt32(0);

	// { ... }
	if (!AxScriptExpressionUnion(this->codeBuilder).Run())
		return false;

	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JMP);
	this->codeBuilder->code->WriteInt32((int)(jumpAddressConditionWhile - this->codeBuilder->code->position - 4));

	long long endCodePos = this->codeBuilder->code->position;
	this->codeBuilder->code->Seek(jumpAddressEndWhile);
	this->codeBuilder->code->WriteInt32((int)(endCodePos - this->codeBuilder->code->position - 4));
	this->codeBuilder->code->Seek(endCodePos);

    return true;
}