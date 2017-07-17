#pragma once

#include "..\..\AxisEngine.h"

class AxOpenGL1_1Texture2D
	: public AxDeviceTexture2D
{
public:
	unsigned int textureId;

	AxOpenGL1_1Texture2D();
	~AxOpenGL1_1Texture2D(void);

	void Update(void *data, int width, int height);
	void GetData(void *data);
};

