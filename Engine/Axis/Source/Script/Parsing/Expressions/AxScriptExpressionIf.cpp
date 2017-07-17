//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionIf.h"

#include "AxScriptExpressionEvaluation.h"
#include "AxScriptExpressionUnion.h"

AxScriptExpressionIf::AxScriptExpressionIf(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionIf::~AxScriptExpressionIf(void)
{
}

bool AxScriptExpressionIf::Run()
{
	// if
	if (!this->codeBuilder->GetToken()->name.Equals("if"))
		return false;
	this->codeBuilder->tokenIndex++;

	// (
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_LeftBracket)
		return false;
	this->codeBuilder->LockTokenRange(this->codeBuilder->tokens.FindClosingBracket(this->codeBuilder->tokenId_RightBracket, this->codeBuilder->tokenIndex) - 1);
	this->codeBuilder->tokenIndex++;

	// x == y
	if (!AxScriptExpressionEvaluation(this->codeBuilder).Run())
		return false;
	this->codeBuilder->tokenIndex--;
	this->codeBuilder->UnlockTokenRange();

	// )
	if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightBracket)
		return false;
	this->codeBuilder->tokenIndex++;

	this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JZ);
	long long jumpAddressEndIf = this->codeBuilder->code->position;
	this->codeBuilder->code->WriteInt32(0);

	// { .. }
	if (!AxScriptExpressionUnion(this->codeBuilder).Run())
		return false;

	// else
	if (this->codeBuilder->GetToken()->name.Equals("else"))
	{
		this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_JMP);
		long long jumpAddressEndElse = this->codeBuilder->code->position;
		this->codeBuilder->code->WriteInt32(0);

		long long endCodePos = this->codeBuilder->code->position;
		this->codeBuilder->code->Seek(jumpAddressEndIf);
		this->codeBuilder->code->WriteInt32((int)(endCodePos - this->codeBuilder->code->position - 4));
		this->codeBuilder->code->Seek(endCodePos);

		this->codeBuilder->tokenIndex++;

		if (!AxScriptExpressionUnion(this->codeBuilder).Run())
			return false;

		endCodePos = this->codeBuilder->code->position;
		this->codeBuilder->code->Seek(jumpAddressEndElse);
		this->codeBuilder->code->WriteInt32((int)(endCodePos - this->codeBuilder->code->position - 4));
		this->codeBuilder->code->Seek(endCodePos);
	}
	else
	{
		long long endCodePos = this->codeBuilder->code->position;
		this->codeBuilder->code->Seek(jumpAddressEndIf);
		this->codeBuilder->code->WriteInt32((int)(endCodePos - this->codeBuilder->code->position - 4));
		this->codeBuilder->code->Seek(endCodePos);
	}

	return true;
}