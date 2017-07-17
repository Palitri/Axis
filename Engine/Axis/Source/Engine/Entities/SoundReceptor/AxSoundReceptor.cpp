//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSoundReceptor.h"

#include "..\..\Axis.h"

AxSoundReceptor::AxSoundReceptor(Axis *context)
	: AxResource()
{
	this->context = context;

	this->typeId = AxSoundReceptor::classId;
	this->resourceType = AxResourceType_SoundReceptor;

	this->deviceSoundReceptor = this->context->audioDevice->CreateSoundReceptor();

	this->paramAddressWorldMatrix = this->deviceSoundReceptor->GetParameterAddress("WorldMatrix");
	this->paramAddressVelocity = this->deviceSoundReceptor->GetParameterAddress("Velocity");
}


AxSoundReceptor::~AxSoundReceptor(void)
{
	delete this->deviceSoundReceptor;
}

void AxSoundReceptor::ApplyWorldLocation(AxMatrix *worldMatrix)
{
	this->deviceSoundReceptor->SetParameter(this->paramAddressWorldMatrix, 0, worldMatrix);
	
	AxMatrix::GetTranslation(this->position, *worldMatrix);
	AxVector3::Subtract(this->velocity, this->position, this->lastPosition);
	this->lastPosition = this->position;

	this->deviceSoundReceptor->SetParameter(this->paramAddressVelocity, 0, &this->velocity);
}
