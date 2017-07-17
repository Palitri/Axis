#pragma once

#include "..\..\AxisEngine.h"

#include "AxDirect3D11GraphicsDevice.h"

class AxDirect3D11Texture2D :
	public AxDeviceTexture2D
{
public:
	int width, height;
	bool isRenderTarget;
	
	void Create(void *data, int width, int height, bool isRenderTarget);
	void Fill(void *data);
	void Release();

	ID3D11Texture2D *deviceTexture;
	ID3D11ShaderResourceView *deviceShaderResourceView;
	ID3D11RenderTargetView *deviceRenderTargetView;
	ID3D11Texture2D *deviceRenderTargetDepthStencil;
	ID3D11DepthStencilView *deviceRenderTargetDepthStencilView;

	ID3D11SamplerState *deviceSampler;

	AxDirect3D11GraphicsDevice *context;

	AxDirect3D11Texture2D(void);
	~AxDirect3D11Texture2D(void);

	void Update(void *data, int width, int height);
	void GetData(void *data);
};

