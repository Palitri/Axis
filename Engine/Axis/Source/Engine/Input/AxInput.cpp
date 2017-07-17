//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxInput.h"


AxInput::AxInput(void)
{
}


AxInput::~AxInput(void)
{
}

AxProperty *AxInput::AcquireInput(const AxString &inputName, AxParameterType inputType)
{
	AxProperty *result = this->GetProperty(inputName);
	
	if (result == 0)
		return this->properties.Add(new AxProperty(inputName, bool(false), inputType));

	if (result->type == inputType)
		return result;

	return 0;
}
