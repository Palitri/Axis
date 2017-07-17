//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxInputModel.h"

#include "..\..\Tools\AxStrings.h"

AxInputModel::AxInputModel(void)
{
}

AxInputModel::~AxInputModel(void)
{
}


void AxInputModel::ParseInputNames(AxInput *input, AxList<AxProperty*> &properties, const AxString &propertyNames)
{
	AxStrings names;
	names.SplitString(AxString(propertyNames.contents), ",");
	for (int i = 0; i < names.count; i++)
	{
		AxProperty *prop = input->GetProperty(names[i]);
		if (prop != 0)
			properties.Add(prop);
	}
}

float AxInputModel::ProcessInputProperties(AxList<AxProperty*> &inputProperties, float deltaTime)
{
	for (int i = 0; i < inputProperties.count; i++)
	{
		AxProperty *inputProperty = inputProperties[i];
		if (inputProperty != 0)
			if (inputProperty->type == AxParameterType_Bool)
			{
				// If value is false, instead of returning, continue checking other alternative inputs
				if (inputProperty->GetBool())
					return 1.0f * deltaTime;
			}
			else if (inputProperty->type == AxParameterType_Float)
			{
				// If value is 0.0, instead of returning, continue checking other alternative inputs
				// If property name contains the word "analog" this means that the input is much like a button with value 0 and 1, but analog - with values in the interval [0, 1] and possibly negative. If so, multiply it by delta time as with digital buttons. The word "analog" stands to denote it's range is [0,1] (or [-1,1]) but including the real numbers in between. It is unlike other analog inputs, such as the mouse, which produce a momentary delta value that generally has no determined range and typically calls for using the value as absolute, in contrast to buttons and analogs, where the value is usually taken in relation to the time it was on
				float value = inputProperty->GetFloat();
				if (value != 0.0f)
					return inputProperty->name.ToLower().IndexOf("analog") == -1 ? value : value * deltaTime;
			}
	}

	return 0.0f;
}
