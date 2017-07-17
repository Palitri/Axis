//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionFor.h"

#include "AxScriptExpressionEvaluation.h"
#include "AxScriptExpressionUnion.h"


AxScriptExpressionFor::AxScriptExpressionFor(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionFor::~AxScriptExpressionFor(void)
{
}

bool AxScriptExpressionFor::Run()
{
	// for
	if (!this->codeBuilder->GetToken()->name.Equals("for"))
		return false;
	this->codeBuilder->tokenIndex++;

	// (
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_LeftBracket)
		return false;
	this->codeBuilder->LockTokenRange(this->codeBuilder->tokens.FindClosingBracket(this->codeBuilder->tokenId_RightBracket, this->codeBuilder->tokenIndex) - 1);
	this->codeBuilder->tokenIndex++;

	// int i = 0;
	if (!AxScriptExpressionUnion(this->codeBuilder).Run())
		return false;

	long long jumpAddressConditionFor = this->codeBuilder->code->position;

	// i < 10;
	if (!AxScriptExpressionEvaluation(this->codeBuilder).Run())
		return false;

	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JZ);
	long long jumpAddressEndFor = this->codeBuilder->code->position;
	this->codeBuilder->code->WriteInt32(0);

	// i++; Performed after the body, so written in another memory and patched later, after the body
	AxMemoryStream *originalCode = this->codeBuilder->code;
	this->codeBuilder->code = new AxMemoryStream();
	if (!AxScriptExpressionUnion(this->codeBuilder).Run())
	{
		delete this->codeBuilder->code;
		this->codeBuilder->code = originalCode;
		return false;
	}
	AxMemoryStream *postOperation = this->codeBuilder->code;
	this->codeBuilder->code = originalCode;
	this->codeBuilder->UnlockTokenRange();

	// )
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightBracket)
		return false;
	this->codeBuilder->tokenIndex++;

	// {...}
	if (!AxScriptExpressionUnion(this->codeBuilder).Run())
		return false;

	postOperation->Seek(0);
	this->codeBuilder->code->WriteStreamData(*postOperation);
	delete postOperation;

	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JMP);
	this->codeBuilder->code->WriteInt32((int)(jumpAddressConditionFor - this->codeBuilder->code->position - 4));

	long long endCodePos = this->codeBuilder->code->position;
	this->codeBuilder->code->Seek(jumpAddressEndFor);
	this->codeBuilder->code->WriteInt32((int)(endCodePos - this->codeBuilder->code->position - 4));
	this->codeBuilder->code->Seek(endCodePos);

	return true;
}