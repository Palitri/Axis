//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionFunctionDeclaration.h"

#include "AxScriptExpressionUnion.h"


AxScriptExpressionFunctionDeclaration::AxScriptExpressionFunctionDeclaration(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionFunctionDeclaration::~AxScriptExpressionFunctionDeclaration(void)
{
}

bool AxScriptExpressionFunctionDeclaration::Run()
{
	// int
	AxScriptDataType *dataType = this->codeBuilder->dataTypes.Find(this->codeBuilder->GetToken()->name);

	if (dataType != 0)
	{
		// functionName
		this->codeBuilder->tokenIndex++;

		if (this->codeBuilder->GetToken()->id == -1)
		{
			AxScriptIdentifier *identifier = this->codeBuilder->identifiers.Find(this->codeBuilder->GetToken()->name);
			if (identifier == 0)
			{
				identifier = this->codeBuilder->identifiers.Add(this->codeBuilder->GetToken()->name, dataType, 1);

				this->codeBuilder->tokenIndex++;

				// (
				if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_LeftBracket)
					return false;
				this->codeBuilder->tokenIndex++;

				// )
				if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightBracket)
					return false;
				this->codeBuilder->tokenIndex++;

				// {...}
				if (AxScriptExpressionUnion(this->codeBuilder).Run())
				{
				}
				else
					return false;

			}
			else
				return false;
		}
		else 
			return false;
	}

	return false;
}