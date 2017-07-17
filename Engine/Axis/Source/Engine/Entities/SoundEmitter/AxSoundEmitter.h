#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

#include "..\..\..\Sound\AxAudioDeviceSoundEmitter.h"

#include "..\..\Primitive\AxVector3.h"

class Axis;

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<int>;

class AXDLLCLASS AxSoundEmitter :
	public AxResource
{
private:
	AxVector3 velocity, position, lastPosition;
	int paramAddressWorldMatrix, paramAddressVelocity;

	AxList<int> propertyDeviceAddresses;

public:
	static const int classId = (AxResourceType_SoundEmitter << 16) | 0;

	static const int propertyIndex_Sound	= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Ambient	= AxResource::propertyIndex_ChildPropertiesIndex + 1;
	static const int propertyIndex_Play		= AxResource::propertyIndex_ChildPropertiesIndex + 2;
	static const int propertyIndex_Loop		= AxResource::propertyIndex_ChildPropertiesIndex + 3;
	static const int propertyIndex_Volume	= AxResource::propertyIndex_ChildPropertiesIndex + 4;
	static const int propertyIndex_Rate		= AxResource::propertyIndex_ChildPropertiesIndex + 5;

	Axis *context;
	AxAudioDeviceSoundEmitter *deviceSoundEmitter;

	AxSoundEmitter(Axis *context);
	virtual ~AxSoundEmitter(void);
	
	void ApplyWorldLocation(AxMatrix *worldMatrix);
	void ApplyProperty(AxProperty *emitterProperty, int deviceAddress);
	void ApplyProperties();
};

