//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionTernaryIf.h"

#include "AxScriptExpressionEvaluation.h"
#include "AxScriptExpressionUnion.h"

AxScriptExpressionTernaryIf::AxScriptExpressionTernaryIf(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionTernaryIf::~AxScriptExpressionTernaryIf(void)
{
}

bool AxScriptExpressionTernaryIf::Run()
{
	// x == y
	this->codeBuilder->LockTokenRange(this->codeBuilder->tokens.FindId(this->codeBuilder->tokenIndex, this->codeBuilder->tokens.count, this->codeBuilder->tokenId_Question) - 1);
	if (!AxScriptExpressionEvaluation(this->codeBuilder).Run())
		return false;
	this->codeBuilder->UnlockTokenRange();
	this->codeBuilder->tokenIndex--;

	// ?
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_Question)
		return false;
	this->codeBuilder->tokenIndex++;

	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JZ);
	long long jumpAddressEndIf = this->codeBuilder->code->position;
	this->codeBuilder->code->WriteInt32(0);

	// x++
	this->codeBuilder->LockTokenRange(this->codeBuilder->tokens.FindId(this->codeBuilder->tokenIndex, this->codeBuilder->tokens.count, this->codeBuilder->tokenId_Colon) - 1);
	if (!AxScriptExpressionUnion(this->codeBuilder).Run())
		return false;
	this->codeBuilder->UnlockTokenRange();
	this->codeBuilder->tokenIndex--;

	// :
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_Colon)
		return false;
	this->codeBuilder->tokenIndex++;

	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JMP);
	long long jumpAddressEndElse = this->codeBuilder->code->position;
	this->codeBuilder->code->WriteInt32(0);

	long long endCodePos = this->codeBuilder->code->position;
	this->codeBuilder->code->Seek(jumpAddressEndIf);
	this->codeBuilder->code->WriteInt32((int)(endCodePos - this->codeBuilder->code->position - 4));
	this->codeBuilder->code->Seek(endCodePos);

	// y++;
	if (!AxScriptExpressionUnion(this->codeBuilder).Run())
		return false;

	endCodePos = this->codeBuilder->code->position;
	this->codeBuilder->code->Seek(jumpAddressEndElse);
	this->codeBuilder->code->WriteInt32((int)(endCodePos - this->codeBuilder->code->position - 4));
	this->codeBuilder->code->Seek(endCodePos);

	return true;
}
