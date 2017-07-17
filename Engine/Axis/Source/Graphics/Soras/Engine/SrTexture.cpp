//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "SrTexture.h"

#include "..\..\..\Tools\AxMem.h"

SrTexture::SrTexture(void)
{
	this->data = 0;

	this->width = 0;
	this->height = 0;
	this->numPixels = 0;
	this->dataSize = 0;
	this->highX = 0;
	this->highY = 0;
}

SrTexture::SrTexture(int width, int height)
{
	this->data = 0;

	this->SetSize(width, height);
}

SrTexture::~SrTexture(void)
{
	this->SetSize(0, 0);
}

void SrTexture::SetSize(int width, int height)
{
	if ((this->width == width) && (this->height == height))
		return;
    
	this->width = width;
    this->height = height;
	this->numPixels = width * height;
	this->dataSize = this->numPixels << 2;
    this->highX = this->width - 2;
    this->highY = this->height - 2;

    if (data != 0)
		delete[] this->data;

	if (this->dataSize != 0)
		this->data = new unsigned char[this->dataSize];
	else
		this->data = 0;
}

void SrTexture::SetPixel(int X, int Y, SrARGB &color)
{
    int linear = (Y * width + X) << 2;
	unsigned char* component = data + linear;
    *(component++) = color.b;
    *(component++) = color.g;
    *(component++) = color.r;
    *(component) = color.a;
}

void SrTexture::GetPixel(int X, int Y, SrARGB &color)
{
    int linear = (Y * width + X) << 2;
	unsigned char* component = data + linear;
    color.b = *(component++);
    color.g = *(component++);
    color.r = *(component++);
    color.a = *(component);
}

void SrTexture::Clear()
{
	AxMem::Zero(this->data, this->dataSize);
}

void SrTexture::Fill(SrARGB color)
{
	unsigned int pixelColor = *(unsigned int*)&color;
	unsigned int *pixel = (unsigned int*)this->data;
	for (int i = this->numPixels; i != 0; i--)
		*pixel++ = pixelColor;

	//AxMem::Zero(pixel, numPixels * 4);
}