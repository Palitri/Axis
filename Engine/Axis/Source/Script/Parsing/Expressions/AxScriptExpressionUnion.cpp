//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionUnion.h"

#include "AxScriptExpressionAssignment.h"
#include "AxScriptExpressionEvaluation.h"
#include "AxScriptExpressionVariableDeclaration.h"
#include "AxScriptExpressionIf.h"
#include "AxScriptExpressionTernaryIf.h"
#include "AxScriptExpressionWhile.h"
#include "AxScriptExpressionFor.h"
#include "AxScriptExpressionDo.h"
#include "AxScriptExpressionCallout.h"

AxScriptExpressionUnion::AxScriptExpressionUnion(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionUnion::~AxScriptExpressionUnion(void)
{
}

bool AxScriptExpressionUnion::Run()
{
	int numBrackets = 0;

	do
	{
		if (this->codeBuilder->GetToken()->id == this->codeBuilder->tokenId_LeftBracket_Big)
		{
			numBrackets++;
			this->codeBuilder->tokenIndex++;
		}
		else if (this->codeBuilder->GetToken()->id ==  this->codeBuilder->tokenId_RightBracket_Big)
		{
			numBrackets--;
			this->codeBuilder->tokenIndex++;
		}
		else
		{
			if (!AxScriptExpressionAssignment(this->codeBuilder).Build())
				if (!AxScriptExpressionEvaluation(this->codeBuilder).Build())
					if (!AxScriptExpressionVariableDeclaration(this->codeBuilder).Build())
						if (!AxScriptExpressionIf(this->codeBuilder).Build())
							if (!AxScriptExpressionTernaryIf(this->codeBuilder).Build())
								if (!AxScriptExpressionWhile(this->codeBuilder).Build())
									if (!AxScriptExpressionFor(this->codeBuilder).Build())
										if (!AxScriptExpressionDo(this->codeBuilder).Build())
											if (!AxScriptExpressionCallout(this->codeBuilder).Build())
												return false;
		}
	}
	while (numBrackets > 0);

	return true;
}