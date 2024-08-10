#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxPropertyChangeNotifiedMechanism.h"

class AxParticleSpreadMechanism :
	public AxPropertyChangeNotifiedMechanism
{
public:
	//static const int classId = AxMechanism::classId | ;

	//static const int propertyIndex_		= AxResource::propertyIndex_ChildPropertiesIndex + 0;

	AxParticleSpreadMechanism(void);
	virtual ~AxParticleSpreadMechanism(void);

	//bool Process(float deltaTime);
};

