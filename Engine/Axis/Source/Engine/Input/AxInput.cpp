//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxInput.h"

#include "..\..\Tools\AxStrings.h"

AxInput::AxInput(void)
{
}


AxInput::~AxInput(void)
{
}

AxProperty *AxInput::AcquireInputControl(const AxString &inputName, AxParameterType inputType)
{
	AxProperty *result = this->GetProperty(inputName);
	
	if (result == 0)
		return this->properties.Add(new AxProperty(inputName, bool(false), inputType));

	if (result->type == inputType)
		return result;

	return 0;
}

void AxInput::GetInputControls(AxInputControls &result, const AxString &inputNames)
{
	AxStrings names;
	names.SplitString(AxString(inputNames.contents), ",");
	for (int i = 0; i < names.count; i++)
	{
		AxProperty *prop = this->GetProperty(names[i]);
		if (prop != 0)
			result.Add(prop);
	}
}
