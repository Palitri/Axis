#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

#include "..\..\..\Sound\AxAudioDeviceSoundReceptor.h"

#include "..\..\Primitive\AxVector3.h"

class Axis;

class AXDLLCLASS AxSoundReceptor :
	public AxResource
{
private:
	AxVector3 velocity, position, lastPosition;
	int paramAddressWorldMatrix, paramAddressVelocity;

public:
	static const int classId = (AxResourceType_SoundReceptor << 16) | 0;

	Axis *context;
	AxAudioDeviceSoundReceptor *deviceSoundReceptor;

	AxSoundReceptor(Axis *context);
	virtual ~AxSoundReceptor(void);

	void ApplyWorldLocation(AxMatrix *worldMatrix);
};

