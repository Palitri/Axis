//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxLinearInterpolationMechanism.h"

#include "..\..\..\Tools\AxMath.h"

AxLinearInterpolationMechanism::AxLinearInterpolationMechanism(float argumentLeft, float argumentRight, float left, float right, bool clampLeft, bool clampRight)
{
	this->typeId = AxLinearInterpolationMechanism::classId;

	this->parameter.SetFloat(left);

    this->properties.Add(new AxProperty("Argument", argumentLeft));
	this->properties.Add(new AxProperty("Argument left", argumentLeft));
    this->properties.Add(new AxProperty("Argument right", argumentRight));
    this->properties.Add(new AxProperty("Left", left));
    this->properties.Add(new AxProperty("Right", right));
    this->properties.Add(new AxProperty("Clamp left", clampLeft));
    this->properties.Add(new AxProperty("Clamp right", clampRight));
}


AxLinearInterpolationMechanism::~AxLinearInterpolationMechanism(void)
{
}

bool AxLinearInterpolationMechanism::Process(float deltaTime)
{
	float argument = *(float*)this->properties[AxLinearInterpolationMechanism::propertyIndex_Argument]->GetEffectiveParameter()->value;
	float argumentLeft = *(float*)this->properties[AxLinearInterpolationMechanism::propertyIndex_ArgumentLeft]->GetEffectiveParameter()->value;
	float argumentRight = *(float*)this->properties[AxLinearInterpolationMechanism::propertyIndex_ArgumentRight]->GetEffectiveParameter()->value;
	float left = *(float*)this->properties[AxLinearInterpolationMechanism::propertyIndex_Left]->GetEffectiveParameter()->value;
	float right = *(float*)this->properties[AxLinearInterpolationMechanism::propertyIndex_Right]->GetEffectiveParameter()->value;
	bool clampLeft = *(bool*)this->properties[AxLinearInterpolationMechanism::propertyIndex_ClampLeft]->GetEffectiveParameter()->value;
	bool clampRight = *(bool*)this->properties[AxLinearInterpolationMechanism::propertyIndex_ClampRight]->GetEffectiveParameter()->value;

	float argumentUnit = (argument - argumentLeft) / (argumentRight - argumentLeft);

	if (clampLeft)
		if (argumentUnit < 0.0f)
			argumentUnit = 0.0f;

	if (clampRight)
		if (argumentUnit > 1.0f)
			argumentUnit = 1.0f;

    *(float*)this->parameter.value = left + (right - left) * argumentUnit;

	return true;
}
