//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFloatValueMechanism.h"


AxFloatValueMechanism::AxFloatValueMechanism(float value)
	: AxMechanism()
{
	this->typeId = AxFloatValueMechanism::classId;

	this->parameter.SetFloat(value);

    this->properties.Add(new AxProperty("Value", value));
}


AxFloatValueMechanism::~AxFloatValueMechanism(void)
{
}

bool AxFloatValueMechanism::Process(float deltaTime)
{
    *(float*)this->parameter.value = *(float*)this->properties[AxFloatValueMechanism::propertyIndex_Value]->GetEffectiveParameter()->value;
    return true;
}