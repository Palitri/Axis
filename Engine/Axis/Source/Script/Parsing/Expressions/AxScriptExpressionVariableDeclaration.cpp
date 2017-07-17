//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionVariableDeclaration.h"

#include "AxScriptExpressionAssignment.h"

AxScriptExpressionVariableDeclaration::AxScriptExpressionVariableDeclaration(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionVariableDeclaration::~AxScriptExpressionVariableDeclaration(void)
{
}

bool AxScriptExpressionVariableDeclaration::Run()
{
	AxScriptDataType *dataType = this->codeBuilder->dataTypes.Find(this->codeBuilder->GetToken()->name);

	if (dataType != 0)
	{
		this->codeBuilder->tokenIndex++;

		int numElements = 1;
		if (this->codeBuilder->GetToken()->id == this->codeBuilder->tokenId_LeftIndexBracket)
		{
			this->codeBuilder->tokenIndex++;
			
			if (!this->codeBuilder->GetToken()->name.ToInt(numElements))
				return false;
			this->codeBuilder->tokenIndex++;

			if (this->codeBuilder->GetToken()->id != this->codeBuilder->tokenId_RightIndexBracket)
				return false;
			this->codeBuilder->tokenIndex++;
		}

		this->codeBuilder->tokenIndex--;
		
		do
		{
			this->codeBuilder->tokenIndex++;

			if (this->codeBuilder->GetToken()->id == -1)
			{
				AxScriptIdentifier *identifier = this->codeBuilder->identifiers.Find(this->codeBuilder->GetToken()->name);
				if (identifier == 0)
				{
					identifier = this->codeBuilder->identifiers.Add(this->codeBuilder->GetToken()->name, dataType, numElements);

					this->codeBuilder->tokenIndex++;

					if (this->codeBuilder->GetToken()->id == this->codeBuilder->tokenId_Set)
					{
						this->codeBuilder->tokenIndex--;
						
						int endToken = this->codeBuilder->tokens.FindId(this->codeBuilder->tokenIndex, this->codeBuilder->tokens.count - 1, this->codeBuilder->tokenId_Comma);
						if (endToken == -1)
							endToken = this->codeBuilder->tokens.FindId(this->codeBuilder->tokenIndex, this->codeBuilder->tokens.count - 1, this->codeBuilder->tokenId_SemiColon);
						if (endToken != -1)
							this->codeBuilder->LockTokenRange(endToken - 1);
						if (!AxScriptExpressionAssignment(this->codeBuilder).Run())
							return false;
						this->codeBuilder->UnlockTokenRange();

						this->codeBuilder->tokenIndex--;
					}

					if (this->codeBuilder->GetToken()->id == this->codeBuilder->tokenId_SemiColon)
					{
						this->codeBuilder->tokenIndex++;
						return true;
					}
				}
				else
					return false;
			}
			else 
				return false;
		}
		while (this->codeBuilder->GetToken()->id == this->codeBuilder->tokenId_Comma);
	}

	return false;
}
