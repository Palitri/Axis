//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDirect3D11GraphicsDevice.h"

#include "AxDirect3D11Mesh.h"
#include "AxDirect3D11Shader.h"
#include "AxDirect3D11Texture2D.h"

#include <Direct3D11\xnamath.h>

AxDirect3D11GraphicsDevice::AxDirect3D11GraphicsDevice(void)
{
	this->deviceDriverType = D3D_DRIVER_TYPE_NULL;
	this->deviceFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	this->device = NULL;
	this->deviceContext = NULL;
	this->deviceSwapChain = NULL;
	this->deviceRenderTargetView = NULL;
	this->deviceRenderTargetDepthStencil = NULL;
	this->deviceRenderTargetDepthStencilView = NULL;

	this->currentRenderTargetView = this->deviceRenderTargetView;
	this->currentRenderTargetDepthStencilView = this->deviceRenderTargetDepthStencilView;

	this->lastShaderTextureBuffersCount = 0;
	this->lastShaderSamplerBuffersCount = 0;

	this->rasterizerState = NULL;

	this->shaderResourceViewsCount = 0;
	this->shaderSamplerStatesCount = 0;

	this->width = 0;
	this->height = 0;
}

AxDirect3D11GraphicsDevice::~AxDirect3D11GraphicsDevice(void)
{
	if (this->deviceContext != NULL)
	{
		this->deviceContext->ClearState();
		this->deviceContext->Flush();
	}

	if (this->rasterizerState != NULL)
		this->rasterizerState->Release();

	if (this->deviceRenderTargetDepthStencilView != NULL)
		this->deviceRenderTargetDepthStencilView->Release();
	if (this->deviceRenderTargetDepthStencil != NULL)
		this->deviceRenderTargetDepthStencil->Release();
	if (this->deviceRenderTargetView != NULL)
		this->deviceRenderTargetView->Release();
	if (this->deviceSwapChain != NULL)
		this->deviceSwapChain->Release();

	if (this->deviceContext != NULL)
		this->deviceContext->Release();
	if (this->device != NULL)
		this->device->Release();
}

AxDeviceMesh *AxDirect3D11GraphicsDevice::CreateMesh()
{
	AxDirect3D11Mesh *mesh = new AxDirect3D11Mesh();
	mesh->context = this;
	return mesh;
}

AxDeviceTexture2D *AxDirect3D11GraphicsDevice::CreateTexture()
{
	AxDirect3D11Texture2D *texture = new AxDirect3D11Texture2D();
	texture->context = this;
	return texture;
}

AxDeviceShader *AxDirect3D11GraphicsDevice::CreateShader()
{
	AxDirect3D11Shader *shader = new AxDirect3D11Shader();
	shader->context = this;
	return shader;
}

bool AxDirect3D11GraphicsDevice::SetRenderTargetWindow(void *windowHandle)
{
	// For switching from render target texture to backbuffer
	if (this->device != 0)
	{
		if (this->currentRenderTargetView == this->deviceRenderTargetView)
			return true;
		
		this->currentRenderTargetView = this->deviceRenderTargetView;
		this->currentRenderTargetDepthStencilView = this->deviceRenderTargetDepthStencilView;
		this->deviceContext->OMSetRenderTargets(1, &this->currentRenderTargetView, this->currentRenderTargetDepthStencilView);
		this->SetViewport(0, 0, this->width, this->height);

		return true;
	}
    
	HRESULT result = S_OK;

	// Set up parameters needed to create the device

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	int windowWidth, windowHeight;

	// Window size
	AxPlatformUtils::GetWindowClientSize(windowHandle, windowWidth, windowHeight);

	// Swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDescription;
    ZeroMemory(&swapChainDescription, sizeof(swapChainDescription));
    swapChainDescription.BufferCount = 1;
    swapChainDescription.BufferDesc.Width = windowWidth;
	swapChainDescription.BufferDesc.Height = windowHeight;
    swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescription.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescription.OutputWindow = *(HWND*)windowHandle;
    swapChainDescription.SampleDesc.Count = 1;
    swapChainDescription.SampleDesc.Quality = 0;
    swapChainDescription.Windowed = TRUE;

	// Driver types
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    // Feature levels
	D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );
	
	// Use the parameters to create a device
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
    {
		this->deviceDriverType = driverTypes[driverTypeIndex];
		result = D3D11CreateDeviceAndSwapChain(
			NULL,
			this->deviceDriverType,
			NULL,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION, 
			&swapChainDescription, 
			&this->deviceSwapChain, 
			&this->device, 
			&this->deviceFeatureLevel, 
			&this->deviceContext);

        if(SUCCEEDED(result))
            break;
    }

	AxDirect3D11GraphicsDevice::SetObjectDebugName(this->deviceContext, AxString("device.deviceContext"));
	AxDirect3D11GraphicsDevice::SetObjectDebugName(this->deviceSwapChain, AxString("device.deviceSwapChain"));
	AxDirect3D11GraphicsDevice::SetObjectDebugName(this->device, AxString("device.deviceSwapChain"));

    if (FAILED(result))
		return false;

	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	this->rasterizerStateDescription.FillMode = D3D11_FILL_SOLID;
	this->rasterizerStateDescription.CullMode = D3D11_CULL_BACK;
	this->rasterizerStateDescription.FrontCounterClockwise = false;
	this->rasterizerStateDescription.DepthBias = 0;
	this->rasterizerStateDescription.SlopeScaledDepthBias = 0.0f;
	this->rasterizerStateDescription.DepthBiasClamp = 0.0f;
	this->rasterizerStateDescription.DepthClipEnable = true;
	this->rasterizerStateDescription.ScissorEnable = false;
	this->rasterizerStateDescription.MultisampleEnable = false;
	this->rasterizerStateDescription.AntialiasedLineEnable = false;
	this->SetRasterizerState();


	this->SetViewport(0, 0, windowWidth, windowHeight);

	return true;
}

bool AxDirect3D11GraphicsDevice::SetRenderTargetTexture(AxDeviceTexture2D *texture)
{

	AxDirect3D11Texture2D *d3dTexture = (AxDirect3D11Texture2D*)texture;

	if (d3dTexture->deviceTexture == NULL)
		return false;

	if (d3dTexture->deviceRenderTargetView == this->currentRenderTargetView)
		return true;

	// Unbind all set shader textures and samplers. Mostly in case the texture is set as input
	for (int i = 0; i < this->shaderResourceViewsCount; i++)
		this->shaderResourceViews[i] = NULL;
	this->deviceContext->PSSetShaderResources(0, this->shaderResourceViewsCount, this->shaderResourceViews);
	this->shaderResourceViewsCount = 0;

	for (int i = 0; i < this->shaderSamplerStatesCount; i++)
		this->shaderSamplerStates[i] = NULL;
	this->deviceContext->PSSetSamplers(0, this->shaderSamplerStatesCount , this->shaderSamplerStates);
	this->shaderSamplerStatesCount = 0;


	if (!d3dTexture->isRenderTarget)
	{
		d3dTexture->Release();
		d3dTexture->Create(0, d3dTexture->width, d3dTexture->height, true);
	}

	this->currentRenderTargetView = d3dTexture->deviceRenderTargetView;
	this->currentRenderTargetDepthStencilView = d3dTexture->deviceRenderTargetDepthStencilView;
	this->deviceContext->OMSetRenderTargets(1, &this->currentRenderTargetView, this->currentRenderTargetDepthStencilView);
	this->SetViewport(0, 0, d3dTexture->width, d3dTexture->height);

	this->ClearScreen(AxVector4(0.5, 0.5, 0.5, 1.0));
	this->ClearDepthBuffer();

	return true;
}

AxPixelFormat AxDirect3D11GraphicsDevice::GetPixelFormat()
{
	return AxPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat::ChannelIdRed, AxPixelFormat::ChannelIdGreen, AxPixelFormat::ChannelIdBlue, AxPixelFormat::ChannelIdAlpha);
}

void AxDirect3D11GraphicsDevice::ClearScreen(AxVector4 &color)
{
	this->deviceContext->ClearRenderTargetView(this->currentRenderTargetView, (FLOAT*)&color);
}

void AxDirect3D11GraphicsDevice::ClearDepthBuffer()
{
	this->deviceContext->ClearDepthStencilView(this->currentRenderTargetDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void AxDirect3D11GraphicsDevice::BeginScene()
{
}

void AxDirect3D11GraphicsDevice::EndScene()
{
}

void AxDirect3D11GraphicsDevice::SetBackBufferSize(int width, int height)
{
	HRESULT result; 

	DXGI_SWAP_CHAIN_DESC swapChainDescription;
	result = this->deviceSwapChain->GetDesc(&swapChainDescription);
	bool isSameSize = (!FAILED(result)) && (swapChainDescription.BufferDesc.Width == width) && (swapChainDescription.BufferDesc.Height == height);
	bool hasDeviceRenderTarget = this->deviceRenderTargetView != NULL;
	
	if (!(isSameSize && hasDeviceRenderTarget))
	{
		this->deviceContext->OMSetRenderTargets(0, 0, 0);

		// Release all outstanding references to the swap chain's buffers.
		if (this->deviceRenderTargetView != NULL)
		{
			this->deviceRenderTargetView->Release();
			this->deviceRenderTargetView = NULL;
		}

		if (!isSameSize)
		{
			result = this->deviceSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
			if (FAILED(result))
				return;
		}

		// Get buffer and create a render-target-view.
		ID3D11Texture2D* pBuffer;
		result = this->deviceSwapChain->GetBuffer(0, __uuidof( ID3D11Texture2D), (void**) &pBuffer);
		if (FAILED(result))
			return;


		result = this->device->CreateRenderTargetView(pBuffer, NULL, &this->deviceRenderTargetView);
		pBuffer->Release();
		if (FAILED(result))
			return ;

		AxDirect3D11GraphicsDevice::SetObjectDebugName(this->deviceRenderTargetView, AxString("device.deviceRenderTargetView"));

		if (this->deviceRenderTargetDepthStencil != NULL)
		{
			this->deviceRenderTargetDepthStencil->Release();
			this->deviceRenderTargetDepthStencil = NULL;
		}

		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC depthStencilDescription;
		AxMem::Zero(&depthStencilDescription, sizeof(depthStencilDescription));
		depthStencilDescription.Width = width;
		depthStencilDescription.Height = height;
		depthStencilDescription.MipLevels = 1;
		depthStencilDescription.ArraySize = 1;
		depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//DXGI_FORMAT_R32_TYPELESS;
		depthStencilDescription.SampleDesc.Count = 1;
		depthStencilDescription.SampleDesc.Quality = 0;
		depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;//D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE;
		depthStencilDescription.CPUAccessFlags = 0;
		depthStencilDescription.MiscFlags = 0;
		if (FAILED(this->device->CreateTexture2D(&depthStencilDescription, NULL, &this->deviceRenderTargetDepthStencil)))
			return ;

		AxDirect3D11GraphicsDevice::SetObjectDebugName(this->deviceRenderTargetDepthStencil, AxString("device.deviceRenderTargetDepthStencil"));

		if (this->deviceRenderTargetDepthStencilView != NULL)
		{
			this->deviceRenderTargetDepthStencilView->Release();
			this->deviceRenderTargetDepthStencilView = NULL;
		}

		// Create the depth stencil view
		D3D11_DEPTH_STENCIL_VIEW_DESC renderTargetDepthStencilViewDescription;
		AxMem::Zero(&renderTargetDepthStencilViewDescription, sizeof(renderTargetDepthStencilViewDescription));
		renderTargetDepthStencilViewDescription.Format = depthStencilDescription.Format;//DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		renderTargetDepthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		renderTargetDepthStencilViewDescription.Texture2D.MipSlice = 0;
		if (FAILED(this->device->CreateDepthStencilView(this->deviceRenderTargetDepthStencil, &renderTargetDepthStencilViewDescription, &this->deviceRenderTargetDepthStencilView)))
			return ;
	
		AxDirect3D11GraphicsDevice::SetObjectDebugName(this->deviceRenderTargetDepthStencilView, AxString("device.deviceRenderTargetDepthStencilView"));

		this->currentRenderTargetView = this->deviceRenderTargetView;
		this->currentRenderTargetDepthStencilView = this->deviceRenderTargetDepthStencilView;
		this->deviceContext->OMSetRenderTargets(1, &this->currentRenderTargetView, this->currentRenderTargetDepthStencilView);
	}

	this->width = width;
	this->height = height;
}

void AxDirect3D11GraphicsDevice::SetViewport(int x, int y, int width, int height)
{
	if ((this->currentRenderTargetView == this->deviceRenderTargetView) && ((width != this->width) || (height != this->height)))
		this->SetBackBufferSize(width, height);

	// Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = (FLOAT)x;
    vp.TopLeftY = (FLOAT)y;
	this->deviceContext->RSSetViewports(1, &vp);
}

void AxDirect3D11GraphicsDevice::SetMesh(AxDeviceMesh &mesh)
{
	this->currentMesh = (AxDirect3D11Mesh*)&mesh;	
    
	UINT stride = this->currentMesh->vertexSize;
    UINT offset = 0;
	this->deviceContext->IASetVertexBuffers(0, 1, &this->currentMesh->vertexBuffer, &stride, &offset);

	this->deviceContext->IASetIndexBuffer(this->currentMesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
}

void AxDirect3D11GraphicsDevice::SetShader(AxDeviceShader &shader)
{
	AxDirect3D11Shader *currentShader = (AxDirect3D11Shader*)&shader;

	this->deviceContext->IASetInputLayout(currentShader->vertexLayout);

	this->deviceContext->VSSetShader(currentShader->vertexShader, NULL, 0);
	this->deviceContext->PSSetShader(currentShader->pixelShader, NULL, 0);

	AxMatrix::Multiply(this->viewProjection, currentShader->viewMatrix, currentShader->projectionMatrix);
	currentShader->SetParameter(currentShader->GetParameterAddress("View projection matrix"), 0, &this->viewProjection);

	for (int i = 0; i <currentShader->constantBuffers.count; i++)
	{
		AxDirect3D11ConstantBuffer *constantBuffer = &currentShader->constantBuffers[i];
		this->deviceContext->UpdateSubresource(constantBuffer->buffer, 0, NULL, constantBuffer->data, 0, 0);

		this->deviceContext->VSSetConstantBuffers(i, 1, &constantBuffer->buffer);
		this->deviceContext->PSSetConstantBuffers(i, 1, &constantBuffer->buffer);
	}

	// Set the new textures and samplers indices according to the current shader and if there were more textures and samplers, set by the previous shader, unbound them by setting their indices to NULL
	for (int i = 0; i < currentShader->shaderTexturesBuffer.count; i++)
		this->shaderResourceViews[i] = currentShader->shaderTexturesBuffer[i];
	for (int i = currentShader->shaderTexturesBuffer.count; i < this->shaderResourceViewsCount; i++)
		this->shaderResourceViews[i] = NULL;
	this->deviceContext->PSSetShaderResources(0, AxMath::Max(currentShader->shaderTexturesBuffer.count, this->shaderResourceViewsCount), this->shaderResourceViews);
	this->shaderResourceViewsCount = currentShader->shaderTexturesBuffer.count;


	for (int i = 0; i < currentShader->shaderSamplersBuffer.count; i++)
		this->shaderSamplerStates[i] = currentShader->shaderSamplersBuffer[i];
	for (int i = currentShader->shaderSamplersBuffer.count; i < this->shaderSamplerStatesCount; i++)
		this->shaderSamplerStates[i] = NULL;
	this->deviceContext->PSSetSamplers(0, AxMath::Max(currentShader->shaderSamplersBuffer.count, this->shaderSamplerStatesCount) , this->shaderSamplerStates);
	this->shaderSamplerStatesCount = currentShader->shaderSamplersBuffer.count;
}


void AxDirect3D11GraphicsDevice::SetRenderState(AxRenderState renderState)
{
	switch (renderState)
	{
		case AxRenderState_CullBackFace:
		{
			if (this->rasterizerStateDescription.CullMode != D3D11_CULL_BACK)
			{
				this->rasterizerStateDescription.CullMode = D3D11_CULL_BACK;
				this->rasterizerStateChanged = true;
			}

			break;
		}

		case AxRenderState_CullFrontFace:
		{
			if (this->rasterizerStateDescription.CullMode != D3D11_CULL_FRONT)
			{
				this->rasterizerStateDescription.CullMode = D3D11_CULL_FRONT;
				this->rasterizerStateChanged = true;
			}

			break;
		}

		case AxRenderState_CullNone:
		{
			if (this->rasterizerStateDescription.CullMode != D3D11_CULL_NONE)
			{
				this->rasterizerStateDescription.CullMode = D3D11_CULL_NONE;
				this->rasterizerStateChanged = true;
			}

			break;
		}

	}
}

void AxDirect3D11GraphicsDevice::RenderMesh()
{
	if (this->rasterizerStateChanged)
		this->SetRasterizerState();

	this->deviceContext->DrawIndexed(this->currentMesh->indexCount, 0, 0);
}

bool AxDirect3D11GraphicsDevice::Present()
{
	this->deviceSwapChain->Present(0, 0);
	return true;
}


void AxDirect3D11GraphicsDevice::SetRasterizerState()
{
	if (this->rasterizerState != NULL)
	{
		this->rasterizerState->Release();
		this->rasterizerState = NULL;
	}

	this->device->CreateRasterizerState(&this->rasterizerStateDescription, &this->rasterizerState);

	this->deviceContext->RSSetState(rasterizerState);

	this->rasterizerStateChanged = false;
}

// Requires D3DCommon.h and dxguid.lib
void AxDirect3D11GraphicsDevice::SetObjectDebugName(ID3D11DeviceChild *deviceObject, AxString &objectName)
{
#if defined(_DEBUG) || defined(PROFILE)
	deviceObject->SetPrivateData(WKPDID_D3DDebugObjectName, objectName.length, objectName.GetCharContents());
#endif
}

// Requires D3DCommon.h and dxguid.lib
void AxDirect3D11GraphicsDevice::SetObjectDebugName(IDXGIObject *deviceObject, AxString &objectName)
{
#if defined(_DEBUG) || defined(PROFILE)
	deviceObject->SetPrivateData(WKPDID_D3DDebugObjectName, objectName.length, objectName.GetCharContents());
#endif
}

// Requires D3DCommon.h and dxguid.lib
void AxDirect3D11GraphicsDevice::SetObjectDebugName(ID3D11Device *deviceObject, AxString &objectName)
{
#if defined(_DEBUG) || defined(PROFILE)
	deviceObject->SetPrivateData(WKPDID_D3DDebugObjectName, objectName.length, objectName.GetCharContents());
#endif
}
