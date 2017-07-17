//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxProperty.h"

AxProperty::AxProperty(const AxString &name, int value)
	: AxParameter(value)
{
	this->name = name;
}

AxProperty::AxProperty(const AxString &name, float value)
	: AxParameter(value)
{
	this->name = name;
}

AxProperty::AxProperty(const AxString &name, bool value)
	: AxParameter(value)
{
	this->name = name;
}

AxProperty::AxProperty(const AxString &name, const AxVector4 &value)
	: AxParameter(value)
{
	this->name = name;
}

AxProperty::AxProperty(const AxString &name, void *value, AxParameterType type)
	: AxParameter(value, type)
{
	this->name = name;
}


AxProperty::~AxProperty(void)
{
}
