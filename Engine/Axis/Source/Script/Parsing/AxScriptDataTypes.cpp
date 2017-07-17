//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptDataTypes.h"


AxScriptDataTypes::AxScriptDataTypes(void)
{
}


AxScriptDataTypes::~AxScriptDataTypes(void)
{
	for (int i = 0; i < this->count; i++)
		delete (*this)[i];
}

AxScriptDataType *AxScriptDataTypes::Find(AxString name)
{
	for (int i = 0; i < this->count; i++)
		if ((*this)[i]->name.Equals(name))
			return (*this)[i];

	return 0;
}
