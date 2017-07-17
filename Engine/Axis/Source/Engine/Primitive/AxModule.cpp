//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxModule.h"


AxModule::AxModule(void)
{
	this->handle = 0;
}

AxModule::AxModule(void *handle)
{
	this->handle = handle;
}

AxModule::AxModule(void *handle, const AxString &name)
{
	this->handle = handle;
	this->name = name;
}


AxModule::~AxModule(void)
{
}
