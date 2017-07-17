#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxDeviceTexture2D.h"

#include "Engine\SrTexture.h"

class AXDLLCLASS AxSorasTexture2D :
	public AxDeviceTexture2D
{
public:
	SrTexture *sorasTexture;

	AxSorasTexture2D(void);
	~AxSorasTexture2D(void);

	virtual void Update(void *data, int width, int height);
	virtual void GetData(void *data);
};

