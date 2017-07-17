//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpression.h"


AxScriptExpression::AxScriptExpression(AxCodeBuilder *codeBuilder)
{
	this->codeBuilder = codeBuilder;
}


AxScriptExpression::~AxScriptExpression(void)
{
}

bool AxScriptExpression::Build()
{
	int startTokenIndex = this->codeBuilder->tokenIndex;
	int startCodePos = (int)this->codeBuilder->code->position;
	int numTokenRangeLocks = this->codeBuilder->tokenRanges.count;
	int numIdentifiers = this->codeBuilder->identifiers.list.count;
	bool result = this->Run();
	if (!result)
	{
		this->codeBuilder->tokenIndex = startTokenIndex;
		this->codeBuilder->code->Seek(startCodePos);
		while (this->codeBuilder->tokenRanges.count > numTokenRangeLocks)
			this->codeBuilder->tokenRanges.Pop();
		for (int i = numIdentifiers; i < this->codeBuilder->identifiers.list.count; i++)
			delete this->codeBuilder->identifiers.list[i];
		this->codeBuilder->identifiers.list.SetSize(numIdentifiers);
	}

	return result;
}
