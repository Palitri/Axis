#pragma once

#include "..\..\AxisEngine.h"

#include "AxOpenALSound.h"

class AxOpenALSoundReceptor :
	public AxAudioDeviceSoundReceptor
{
private:
	static const int parameterAddress_Invalid		= -1;
	static const int parameterAddress_WorldMatrix	= 1;
	static const int parameterAddress_Velocity		= 2;

public:
	AxOpenALSoundReceptor(void);
	~AxOpenALSoundReceptor(void);

	virtual int GetParameterAddress(AxString parameterName);
	virtual void SetParameter(int address, int index, void *value);
};

