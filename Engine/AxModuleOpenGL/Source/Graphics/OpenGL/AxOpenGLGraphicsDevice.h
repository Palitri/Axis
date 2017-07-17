#pragma once

#include "..\..\AxisEngine.h"

#include <windows.h>
#include <OpenGL\glew.h>


class AxOpenGLMesh;
class AxOpenGLShader;
class AxOpenGLTexture2D;

class AxOpenGLGraphicsDevice
	: public AxGraphicsDevice
{
private:
	AxOpenGLMesh *currentMesh;
	AxOpenGLShader *currentShader;
	AxOpenGLTexture2D *currentRenderTarget;

	int width, height;

	AxRenderState cullMode;
    
	AxPixelFormat pixelFormat;

	int maxBones;
	AxMatrix *bones;

	void *windowHandle_HWND, *deviceContext_HDC, *renderContext_HGLRC;
	
public:
	AxOpenGLGraphicsDevice();
	~AxOpenGLGraphicsDevice(void);

	AxDeviceMesh *CreateMesh();
	AxDeviceTexture2D *CreateTexture();
	AxDeviceShader *CreateShader();

	bool SetRenderTargetWindow(void *windowHandle);
	bool SetRenderTargetTexture(AxDeviceTexture2D *texture);
	
	AxPixelFormat GetPixelFormat();

	void ClearScreen(AxVector4 &color);
	void ClearDepthBuffer();

	void BeginScene();
	void EndScene();
	
	void SetViewport(int x, int y, int width, int height);
	
	void SetMesh(AxDeviceMesh &mesh);

	void SetShader(AxDeviceShader &shader);

	void SetRenderState(AxRenderState renderState);

	void RenderMesh();

	bool Present();
};
