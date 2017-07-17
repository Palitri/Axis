//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSorasGraphicsDevice.h"

#include <windows.h>

#include "AxSorasMesh.h"
#include "AxSorasTexture2D.h"
#include "AxSorasShader.h"

#include "..\..\Engine\Entities\Textures\AxTexture2D.h"

#include "..\..\Tools\Platform\AxPlatformUtils.h"

AxSorasGraphicsDevice::AxSorasGraphicsDevice()
{
	this->windowHandle_HWND = new HWND();
	this->deviceContext_HDC = new HDC();

	this->drawSurface = new AxDrawSurface();
	this->soras = new Soras();
	this->soras->SetRenderSurface(new SrTexture());

	this->pixelFormat = AxPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat::ChannelIdBlue, AxPixelFormat::ChannelIdGreen, AxPixelFormat::ChannelIdRed, AxPixelFormat::ChannelIdAlpha);
}


AxSorasGraphicsDevice::~AxSorasGraphicsDevice(void)
{
	delete this->soras->renderSurface;
	delete this->soras;
	delete this->drawSurface;

	delete this->windowHandle_HWND;
	delete this->deviceContext_HDC;
}

AxDeviceMesh *AxSorasGraphicsDevice::CreateMesh()
{
	return new AxSorasMesh();
}

AxDeviceTexture2D *AxSorasGraphicsDevice::CreateTexture()
{
	return new AxSorasTexture2D();
}

AxDeviceShader *AxSorasGraphicsDevice::CreateShader()
{
	AxSorasShader *shader = new AxSorasShader();
	shader->context = this;
	return shader;
}

bool AxSorasGraphicsDevice::SetRenderTargetWindow(void *windowHandle)
{
	if (*(HWND*)this->windowHandle_HWND == *(HWND*)windowHandle)
		return true;
	
	*(HWND*)this->windowHandle_HWND = *(HWND*)windowHandle;
	*(HDC*)this->deviceContext_HDC = GetDC(*(HWND*)this->windowHandle_HWND);

	int width, height;
	AxPlatformUtils::GetWindowClientSize(windowHandle, width, height);

	this->SetViewport(0, 0, width, height);

	return true;
}

bool AxSorasGraphicsDevice::SetRenderTargetTexture(AxDeviceTexture2D *texture)
{
	return false;
}

AxPixelFormat AxSorasGraphicsDevice::GetPixelFormat()
{
	return this->pixelFormat;
}

void AxSorasGraphicsDevice::ClearScreen(AxVector4 &color)
{
	this->soras->Clear(SrARGB((unsigned char)(color.w * 0xff), (unsigned char)(color.x * 0xff), (unsigned char)(color.y * 0xff), (unsigned char)(color.z * 0xff)));
}

void AxSorasGraphicsDevice::ClearDepthBuffer()
{
	this->soras->ClearZ();
}

void AxSorasGraphicsDevice::BeginScene()
{
}

void AxSorasGraphicsDevice::EndScene()
{
}

void AxSorasGraphicsDevice::SetViewport(int x, int y, int width, int height)
{
	this->soras->renderSurface->SetSize(width, height);
	this->soras->SetRenderSurface(this->soras->renderSurface);
	this->drawSurface->SetSize(width, height);
}

void AxSorasGraphicsDevice::SetMesh(AxDeviceMesh &mesh)
{
	this->soras->SetMesh(((AxSorasMesh*)(&mesh))->sorasMesh);
}

void AxSorasGraphicsDevice::SetShader(AxDeviceShader &shader)
{
	AxSorasShader *srShader = (AxSorasShader*)&shader;

	this->soras->ClearShaderOperations();
	
	for (int i = 0; i < srShader->vertexShaderOps.count; i++)
	{
		this->soras->AddVertexShader(srShader->vertexShaderOps[i], srShader->vertexColorOps[i]);
	}

	for (int i = 0; i < srShader->pixelShaderOps.count; i++)
	{
		this->soras->AddPixelShader(srShader->pixelShaderOps[i], srShader->pixelColorOps[i], srShader->pixelOpsTextures[i]);
	}

	AxMatrix::Multiply(this->viewProjection, srShader->viewMatrix, srShader->projectionMatrix);
	this->soras->viewProjectionMatrix = *(SrMatrix*)&this->viewProjection;

	this->soras->skeletalAnimationOn = srShader->isAnimated;
}

void AxSorasGraphicsDevice::SetRenderState(AxRenderState renderState)
{
}

void AxSorasGraphicsDevice::RenderMesh()
{
	this->soras->RenderMesh();
}

bool AxSorasGraphicsDevice::Present()
{
	this->drawSurface->Copy(this->soras->renderSurface->data);
	this->drawSurface->Present(0, 0, *(HDC*)this->deviceContext_HDC);

	return true;
}
