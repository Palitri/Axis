//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenALSoundReceptor.h"

#include "..\..\AxisEngine.h"

#include <OpenAL\al.h>

//Can OpenAL have multiple listeners?
//Technically, OpenAL Soft does support it. You can't get multiple listeners in 
//a single context, but you can create multiple contexts from a single device 
//and each context will have one listener. All contexts on a given device will 
//go into a single mix, so there won't be any added latency. The contexts can 
//share buffers (as they're on the same device), but not sources (each context 
//has its own list of sources).

AxOpenALSoundReceptor::AxOpenALSoundReceptor(void)
{
}


AxOpenALSoundReceptor::~AxOpenALSoundReceptor(void)
{
}

int AxOpenALSoundReceptor::GetParameterAddress(AxString parameterName)
{
	if (parameterName.Equals("WorldMatrix"))
		return AxOpenALSoundReceptor::parameterAddress_WorldMatrix;
	else if (parameterName.Equals("Velocity"))
		return AxOpenALSoundReceptor::parameterAddress_Velocity;

	return AxOpenALSoundReceptor::parameterAddress_Invalid;
}

void AxOpenALSoundReceptor::SetParameter(int address, int index, void *value)
{
	if (address == AxOpenALSoundReceptor::parameterAddress_Invalid)
		return;

	switch (address)
	{
		// World Matrix
		case AxOpenALSoundReceptor::parameterAddress_WorldMatrix:
		{
			if (value == 0)
				break;

			AxVector3 orientation[2];
			AxMatrix::GetAxisZ(orientation[0], *(AxMatrix*)value);
			AxMatrix::GetAxisY(orientation[1], *(AxMatrix*)value);

			alListenerfv(AL_POSITION, (ALfloat*)&((AxMatrix*)value)->_41);
			alListenerfv(AL_ORIENTATION, (ALfloat*)&orientation);

			break;
		}

		// Velocity
		case AxOpenALSoundReceptor::parameterAddress_Velocity:
		{
			if (value != 0)
				alListenerfv(AL_VELOCITY, (ALfloat*)value);

			break;
		}
	}
}