//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDrawSurface.h"

#include "..\..\Tools\AxMem.h"

AxDrawSurface::AxDrawSurface(void)
{
	this->pixelData = 0;
	this->width = 0;
	this->height = 0;
	this->pixelDataSize = 0;
}

AxDrawSurface::~AxDrawSurface(void)
{
	this->SetSize(0, 0);
}

void AxDrawSurface::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
	this->pixelDataSize = this->width * this->height * 4;

	this->bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	this->bitmapInfo.bmiHeader.biWidth = width;
	this->bitmapInfo.bmiHeader.biHeight = height;
	this->bitmapInfo.bmiHeader.biPlanes = 1;
	this->bitmapInfo.bmiHeader.biBitCount = 32;
	this->bitmapInfo.bmiHeader.biCompression = BI_RGB;
	this->bitmapInfo.bmiHeader.biSizeImage = 0;
	this->bitmapInfo.bmiHeader.biXPelsPerMeter = 0;
	this->bitmapInfo.bmiHeader.biYPelsPerMeter = 0;
	this->bitmapInfo.bmiHeader.biClrUsed = 0;
	this->bitmapInfo.bmiHeader.biClrImportant = 0;

	if (this->pixelData != 0)
	{
		VirtualFree(this->pixelData, 0, MEM_RELEASE);
		this->pixelData = 0;
	}

	if (this->pixelDataSize > 0)
	{
		this->pixelData = VirtualAlloc(0, this->pixelDataSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);	
	}
}

void AxDrawSurface::Copy(void* pixelData)
{
	AxMem::Copy(this->pixelData, pixelData, this->pixelDataSize);
}

void AxDrawSurface::Present(int x, int y, HDC deviceContext)
{
	SetDIBitsToDevice(
		deviceContext,
		x, y, this->width, this->height,
		0, 0,
		0, this->height,
		this->pixelData,
		&this->bitmapInfo,
		DIB_RGB_COLORS);
}
