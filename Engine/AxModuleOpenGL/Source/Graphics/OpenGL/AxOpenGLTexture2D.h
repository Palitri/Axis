#pragma once

#include "..\..\AxisEngine.h"

#include <OpenGL\glew.h>

#include "AxOpenGLGraphicsDevice.h"

class AxOpenGLTexture2D
	: public AxDeviceTexture2D
{
public:
	AxOpenGLGraphicsDevice *context;

	GLuint glTexture, glDepthBuffer, glFrameBuffer;

	bool isFrameBuffer;
	int width, height;

	void Create(void *data, int width, int height, bool isRenderTarget);
	void Dispose();

	AxOpenGLTexture2D();
	~AxOpenGLTexture2D(void);

	void Update(void *data, int width, int height);
	void GetData(void *data);
};

