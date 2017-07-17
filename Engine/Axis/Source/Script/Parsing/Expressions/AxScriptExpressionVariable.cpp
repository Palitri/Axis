//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionVariable.h"

#include "..\AxScriptIdentifier.h"
#include "AxScriptExpressionEvaluation.h"

AxScriptExpressionVariable::AxScriptExpressionVariable(AxCodeBuilder *codeBuilder, bool assignmentMode) :
	AxScriptExpression(codeBuilder)
{
	this->assignmentMode = assignmentMode;
}


AxScriptExpressionVariable::~AxScriptExpressionVariable(void)
{
}

bool AxScriptExpressionVariable::Run()
{
	AxScriptIdentifier *identifier = this->codeBuilder->identifiers.Find(this->codeBuilder->GetToken()->name);
	
	if (identifier != 0)
	{
		this->codeBuilder->tokenIndex++;

		bool indexed = false;
		if (this->codeBuilder->GetToken()->id == this->codeBuilder->tokenId_LeftIndexBracket)
		{
			this->codeBuilder->LockTokenRange(this->codeBuilder->tokens.FindClosingBracket(this->codeBuilder->tokenId_RightIndexBracket, this->codeBuilder->tokenIndex) - 1);
			this->codeBuilder->tokenIndex++;
			if (!AxScriptExpressionEvaluation(this->codeBuilder).Run())
				return false;
			this->codeBuilder->UnlockTokenRange();

			this->codeBuilder->tokenIndex--;
			if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightIndexBracket)
				return false;
			this->codeBuilder->tokenIndex++;

			indexed = true;
		}

		if (this->assignmentMode)
		{
			this->codeBuilder->dataType = identifier->dataType;

			// Set an operation that puts the address of the variable into the stack
			if (indexed)
			{
				if (identifier->dataType == this->codeBuilder->dataType_int)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_IPushIndexedAddress);
				else if (identifier->dataType == this->codeBuilder->dataType_float)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_FPushIndexedAddress);
			}
			else
			{
				if (identifier->dataType == this->codeBuilder->dataType_int)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_IPushAddress);
				else if (identifier->dataType == this->codeBuilder->dataType_float)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_FPushAddress);
			}

			this->codeBuilder->code->WriteUInt32(identifier->address);
		}
		else
		{
			// Set an operation that puts the value of the variable's address into the stack
			if (indexed)
			{
				if (identifier->dataType == this->codeBuilder->dataType_int)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_IPushIndexedAddressValue);
				else if (identifier->dataType == this->codeBuilder->dataType_float)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_FPushIndexedAddressValue);
			}
			else
			{
				if (identifier->dataType == this->codeBuilder->dataType_int)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_IPushAddressValue);
				else if (identifier->dataType == this->codeBuilder->dataType_float)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_FPushAddressValue);
			}

			this->codeBuilder->code->WriteUInt32(identifier->address);

			// Convert data type if necessary
			if (identifier->dataType == this->codeBuilder->dataType_float)
				if (this->codeBuilder->dataType == this->codeBuilder->dataType_int)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_FToI);

			if (identifier->dataType == this->codeBuilder->dataType_int)
				if (this->codeBuilder->dataType == this->codeBuilder->dataType_float)
					this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_IToF);
		}

		return true;
	}

	return false;
}
