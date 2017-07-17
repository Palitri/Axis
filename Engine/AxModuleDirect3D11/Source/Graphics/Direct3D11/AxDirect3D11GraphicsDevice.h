#pragma once

#include "..\..\AxisEngine.h"

#include <windows.h>

#include <Direct3D11\D3D11.h>

class AxDirect3D11Mesh;
class AxDirect3D11Shader;
class AxDirect3D11Texture2D;

class AxDirect3D11GraphicsDevice :
	public AxGraphicsDevice
{
private:
	static const int DeviceMaxShaderResourceViews = 128;
	static const int DeviceMaxShaderSamplerStates = 128;

	int width, height;

	// TODO: WARNING: When a new scene gets loaded, the "current"Mesh, Shader and etc. get deleted when the old scene's resources are destroyed
	AxDirect3D11Mesh *currentMesh;
	//AxDirect3D11Shader *currentShader;
	//AxDirect3D11Texture2D *currentRenderTarget;
	ID3D11RenderTargetView *currentRenderTargetView;
	ID3D11DepthStencilView *currentRenderTargetDepthStencilView;

	int lastShaderTextureBuffersCount, lastShaderSamplerBuffersCount;

	D3D11_RASTERIZER_DESC rasterizerStateDescription;
	ID3D11RasterizerState *rasterizerState;
	bool rasterizerStateChanged;

	AxMatrix viewProjection;

	ID3D11ShaderResourceView *shaderResourceViews[AxDirect3D11GraphicsDevice::DeviceMaxShaderResourceViews];
	ID3D11SamplerState *shaderSamplerStates[AxDirect3D11GraphicsDevice::DeviceMaxShaderSamplerStates];
	int shaderResourceViewsCount, shaderSamplerStatesCount;

	void SetBackBufferSize(int width, int height);
	void SetRasterizerState();

public:
	static void SetObjectDebugName(ID3D11DeviceChild *deviceObject, AxString &objectName);
	static void SetObjectDebugName(IDXGIObject *deviceObject, AxString &objectName);
	static void SetObjectDebugName(ID3D11Device	*deviceObject, AxString &objectName);

	D3D_DRIVER_TYPE         deviceDriverType;
	D3D_FEATURE_LEVEL       deviceFeatureLevel;
	ID3D11Device*           device;
	ID3D11DeviceContext*    deviceContext;
	IDXGISwapChain*         deviceSwapChain;
	ID3D11RenderTargetView* deviceRenderTargetView;
	ID3D11Texture2D*		deviceRenderTargetDepthStencil;
	ID3D11DepthStencilView* deviceRenderTargetDepthStencilView;


	AxDirect3D11GraphicsDevice(void);
	~AxDirect3D11GraphicsDevice(void);

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

