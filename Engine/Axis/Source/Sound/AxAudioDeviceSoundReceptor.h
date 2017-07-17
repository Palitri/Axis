#pragma once

#include "..\AxGlobals.h"

#include "..\Tools\AxString.h"

class AXDLLCLASS AxAudioDeviceSoundReceptor
{
public:
	AxAudioDeviceSoundReceptor(void);
	~AxAudioDeviceSoundReceptor(void);

	virtual int GetParameterAddress(AxString parameterName) = 0;
	virtual void SetParameter(int address, int index, void *value) = 0;
};

