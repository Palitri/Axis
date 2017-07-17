//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSineMechanism.h"

#include "..\..\..\Tools\AxMath.h"

AxSineMechanism::AxSineMechanism(float periodStart, float periodEnd, float amplitudeMin, float amplitudeMax)
	: AxMechanism()
{
	this->typeId = AxSineMechanism::classId;

	this->parameter.SetFloat(0.0f);

	this->properties.Add(new AxProperty("Argument", periodStart));
    this->properties.Add(new AxProperty("Period start", periodStart));
    this->properties.Add(new AxProperty("Period end", periodEnd));
    this->properties.Add(new AxProperty("Amplitude min", amplitudeMin));
    this->properties.Add(new AxProperty("Amplitude max", amplitudeMax));
}


AxSineMechanism::~AxSineMechanism(void)
{
}


bool AxSineMechanism::Process(float deltaTime)
{
	float argument = *(float*)this->properties[AxSineMechanism::propertyIndex_Argument]->GetEffectiveValue();
	float periodStart = *(float*)this->properties[AxSineMechanism::propertyIndex_PeriodStart]->GetEffectiveValue();
	float periodEnd = *(float*)this->properties[AxSineMechanism::propertyIndex_PeriodEnd]->GetEffectiveValue();
	float amplitudeMin = *(float*)this->properties[AxSineMechanism::propertyIndex_AmplitudeMin]->GetEffectiveValue();
	float amplitudeMax = *(float*)this->properties[AxSineMechanism::propertyIndex_AmplitudeMax]->GetEffectiveValue();

	float angle = AxMath::Pi2 * (argument - periodStart) / (periodEnd - periodStart);

	*(float*)this->parameter.value = amplitudeMin + (amplitudeMax - amplitudeMin) * (AxMath::Sin(angle) + 1.0f) / 2.0f;

	return true;
}