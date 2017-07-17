//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptDataType.h"


AxScriptDataType::AxScriptDataType(AxString name)
{
	this->name = name;
	this->size = 0;
}

AxScriptDataType::AxScriptDataType(AxString name, int size)
{
	this->name = name;
	this->size = size;
}


AxScriptDataType::~AxScriptDataType(void)
{
}

void AxScriptDataType::AddField(AxScriptDataType *field)
{
	this->fields.Add(field);
	this->size += field->size;
}
