//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptToken.h"


AxScriptToken::AxScriptToken(void)
{
}

AxScriptToken::AxScriptToken(AxString name, int id, int start, int end)
{
	this->name = name;
	this->id = id;
	this->start = start;
	this->end = end;
}


AxScriptToken::~AxScriptToken(void)
{
}
