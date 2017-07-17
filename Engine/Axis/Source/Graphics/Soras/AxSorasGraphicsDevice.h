#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxGraphicsDevice.h"

#include "Engine\Soras.h"

#include "..\..\Tools\Platform\AxDrawSurface.h"

class AXDLLCLASS AxSorasGraphicsDevice :
	public AxGraphicsDevice
{
private:
	AxMatrix viewProjection;

	void *windowHandle_HWND;
	void *deviceContext_HDC;

	AxPixelFormat pixelFormat;

public:
	Soras *soras;
	AxDrawSurface *drawSurface;

	AxSorasGraphicsDevice();
	~AxSorasGraphicsDevice(void);

	virtual AxDeviceMesh *CreateMesh();
	virtual AxDeviceTexture2D *CreateTexture();
	virtual AxDeviceShader *CreateShader();

	virtual bool SetRenderTargetWindow(void *windowHandle);
	virtual bool SetRenderTargetTexture(AxDeviceTexture2D *texture);

	virtual AxPixelFormat GetPixelFormat();

	virtual void ClearScreen(AxVector4 &color);
	virtual void ClearDepthBuffer();

	virtual void BeginScene();
	virtual void EndScene();
	
	virtual void SetViewport(int x, int y, int width, int height);
	
	virtual void SetMesh(AxDeviceMesh &mesh);

	virtual void SetShader(AxDeviceShader &shader);

	virtual void SetRenderState(AxRenderState renderState);

	virtual void RenderMesh();

	virtual bool Present();
};

