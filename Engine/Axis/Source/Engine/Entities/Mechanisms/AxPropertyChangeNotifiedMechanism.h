#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxParameter*>;

class AXDLLCLASS AxPropertyChangeNotifiedMechanism :
	public AxMechanism
{
public:
	static const int classId = (AxResourceType_Mechanism << 16) | 1;

	AxPropertyChangeNotifiedMechanism(void);
	virtual ~AxPropertyChangeNotifiedMechanism(void);

	virtual bool Process(float deltaTime);
};

