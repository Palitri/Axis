//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptIdentifier.h"


AxScriptIdentifier::AxScriptIdentifier(AxString name, AxScriptDataType* dataType, int address)
{
	this->name = name;
	this->dataType = dataType;
	this->address = address;
}


AxScriptIdentifier::~AxScriptIdentifier(void)
{
}
