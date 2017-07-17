#pragma once

#include "..\..\AxisEngine.h"

#include <windows.h>
#include <gl\GL.h>

class AxOpenGL1_1Mesh;
class AxOpenGL1_1Shader;

class AxOpenGL1_1GraphicsDevice
	: public AxGraphicsDevice
{
private:
	AxOpenGL1_1Mesh *currentMesh;
	AxOpenGL1_1Shader *currentShader;
    
	AxPixelFormat pixelFormat;

	void *windowHandle_HWND, *deviceContext_HDC, *renderContext_HGLRC;
	int pixelFormatIndex;
	
	AxOpenGL1_1Mesh *animatedMesh;

	// Fills the animatedMesh member variable with vertex positions and normals affected by skinning
	void CreateAnimatedMesh(AxOpenGL1_1Mesh *glMesh);

public:
	AxMatrix *bones;
	int maxBones;

	AxOpenGL1_1GraphicsDevice();
	~AxOpenGL1_1GraphicsDevice(void);

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
