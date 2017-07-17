//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionAssignment.h"

#include "AxScriptExpressionVariable.h"
#include "AxScriptExpressionEvaluation.h"
#include "AxScriptExpressionUnion.h"

AxScriptExpressionAssignment::AxScriptExpressionAssignment(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionAssignment::~AxScriptExpressionAssignment(void)
{
}

bool AxScriptExpressionAssignment::Run()
{
	if (!AxScriptExpressionVariable(this->codeBuilder, true).Run())
		return false;

	int operationId = this->codeBuilder->GetTokenAssignmentOperation();
	if (operationId == -1)
		return false;

	this->codeBuilder->tokenIndex++;

	if (this->codeBuilder->GetToken() != this->codeBuilder->invalidToken)
		if (this->codeBuilder->GetToken()->id == this->codeBuilder->tokenId_SemiColon)
			this->codeBuilder->tokenIndex++;
		else
			//if (!AxScriptExpressionUnion(this->codeBuilder).Run())
			if (!AxScriptExpressionEvaluation(this->codeBuilder).Run())
				return false;

	this->codeBuilder->code->WriteUInt8(operationId);
	
	return true;
}