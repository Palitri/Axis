#pragma once

#include "..\AxGlobals.h"

class AXDLLCLASS AxDeviceTexture2D
{
public:
	AxDeviceTexture2D(void);
	virtual ~AxDeviceTexture2D(void);

	// Updates the texture with the provided pixel data
	// Pixel data is packed without any padding. Pixel format is as of the respective graphics device
	virtual void Update(void *data, int width, int height) = 0;
	// Gets the pixel data of the texture
	// The data is returned packed, without any padding. The pixel format is as of the respective graphics device
	virtual void GetData(void *data) = 0;
};

