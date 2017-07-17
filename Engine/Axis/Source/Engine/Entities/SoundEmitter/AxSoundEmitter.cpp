//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSoundEmitter.h"

#include "..\..\Axis.h"

AxSoundEmitter::AxSoundEmitter(Axis *context)
	: AxResource()
{
	this->context = context;

	this->typeId = AxSoundEmitter::classId;
	this->resourceType = AxResourceType_SoundEmitter;

	this->deviceSoundEmitter = this->context->audioDevice->CreateSoundEmitter();

	this->properties.Add(new AxProperty(AxString("Sound"), 0, AxParameterType_ReferenceSound));
    this->properties.Add(new AxProperty(AxString("Ambient"), false));
    this->properties.Add(new AxProperty(AxString("Play"), true));
    this->properties.Add(new AxProperty(AxString("Loop"), true));
    this->properties.Add(new AxProperty(AxString("Volume"), 1.0f));
    this->properties.Add(new AxProperty(AxString("Rate"), 1.0f));

	this->propertyDeviceAddresses.SetSize(this->properties.count);
	for (int i = 0; i < this->properties.count; i++)
		this->propertyDeviceAddresses[i] = this->deviceSoundEmitter->GetParameterAddress(this->properties[i]->name);

	this->paramAddressWorldMatrix = this->deviceSoundEmitter->GetParameterAddress("WorldMatrix");
	this->paramAddressVelocity = this->deviceSoundEmitter->GetParameterAddress("Velocity");
}


AxSoundEmitter::~AxSoundEmitter(void)
{
	delete this->deviceSoundEmitter;
}

void AxSoundEmitter::ApplyProperty(AxProperty *emitterProperty, int deviceAddress)
{
	switch (emitterProperty->type)
	{
		case AxParameterType_ReferenceSound:
		{
			AxSound *sound = (AxSound*)emitterProperty->GetEffectiveValue();
			if (sound != 0)
				this->deviceSoundEmitter->SetParameter(deviceAddress, 0, sound->deviceSound);
			break;
		}

		default:
		{
			this->deviceSoundEmitter->SetParameter(deviceAddress, 0, emitterProperty->GetEffectiveValue());
			break;
		}
	}
}

void AxSoundEmitter::ApplyProperties()
{
	int count = this->properties.count;
	for (int i = 0; i < count; i++)
		this->ApplyProperty(this->properties[i], this->propertyDeviceAddresses[i]);
}

void AxSoundEmitter::ApplyWorldLocation(AxMatrix *worldMatrix)
{
	this->deviceSoundEmitter->SetParameter(this->paramAddressWorldMatrix, 0, worldMatrix);
	
	AxMatrix::GetTranslation(this->position, *worldMatrix);
	AxVector3::Subtract(this->velocity, this->position, this->lastPosition);
	this->lastPosition = this->position;

	this->deviceSoundEmitter->SetParameter(this->paramAddressVelocity, 0, &this->velocity);
}
