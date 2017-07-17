#pragma once

#include "..\AxGlobals.h"

#include "..\Engine\Primitive\AxShadingOperation.h"
#include "..\Engine\Primitive\AxColorOperation.h"
#include "..\Engine\Primitive\AxRenderState.h"
#include "..\Engine\Primitive\AxVector4.h"
#include "..\Engine\Primitive\AxMatrix.h"
#include "..\Engine\Primitive\AxPixelFormat.h"

#include "AxDeviceMesh.h"
#include "AxDeviceTexture2D.h"
#include "AxDeviceShader.h"


class AXDLLCLASS AxGraphicsDevice
{
public:
	AxGraphicsDevice(void);
	virtual ~AxGraphicsDevice(void);

	virtual AxDeviceMesh *CreateMesh() = 0;
	virtual AxDeviceTexture2D *CreateTexture() = 0;
	virtual AxDeviceShader *CreateShader() = 0;

	virtual bool SetRenderTargetWindow(void *windowHandle) = 0;
	virtual bool SetRenderTargetTexture(AxDeviceTexture2D *texture) = 0;

	virtual AxPixelFormat GetPixelFormat() = 0;

	virtual void ClearScreen(AxVector4 &color) = 0;
	virtual void ClearDepthBuffer() = 0;

	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
	
	virtual void SetViewport(int x, int y, int width, int height) = 0;
	
	virtual void SetMesh(AxDeviceMesh &mesh) = 0;

	virtual void SetShader(AxDeviceShader &shader) = 0;

	virtual void SetRenderState(AxRenderState renderState) = 0;

	virtual void RenderMesh() = 0;

	virtual bool Present() = 0;
};

