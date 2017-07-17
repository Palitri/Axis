#pragma once

#include "..\..\AxisEngine.h"

class AxOpenGL1_1GraphicsDevice;

class AxOpenGL1_1Shader :
	public AxDeviceIndependentShader
{
public:
	AxOpenGL1_1GraphicsDevice *context;

	AxMatrix modelTransform, viewTransform, projectionTransform,
    		 modelViewTransform;

	bool isAnimated;

	AxOpenGL1_1Shader(void);
	~AxOpenGL1_1Shader(void);
	
	virtual int GetParameterAddress(AxString parameterName);
	virtual void SetParameter(int address, int index, void *value);

	virtual bool Update(AxString &compilationMessage);
};

