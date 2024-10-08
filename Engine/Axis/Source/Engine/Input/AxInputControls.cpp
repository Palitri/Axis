#include "AxInputControls.h"

AxInputControls::AxInputControls(void)
{
}


AxInputControls::~AxInputControls(void)
{
}

float AxInputControls::GetValue(float deltaTime)
{
	for (int i = 0; i < this->count; i++)
	{
		AxProperty *inputProperty = this->Get(i);
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
