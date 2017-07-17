//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSorasTexture2D.h"

#include "..\..\Tools\AxMem.h"

AxSorasTexture2D::AxSorasTexture2D(void)
{
	this->sorasTexture = new SrTexture();
}


AxSorasTexture2D::~AxSorasTexture2D(void)
{
	delete this->sorasTexture;
}

void AxSorasTexture2D::Update(void *data, int width, int height)
{
	this->sorasTexture->SetSize(width, height);

	if (data != 0)
		AxMem::Copy(this->sorasTexture->data, data, this->sorasTexture->dataSize);
}

void AxSorasTexture2D::GetData(void *data)
{
	AxMem::Copy(data, this->sorasTexture->data, this->sorasTexture->dataSize);
}
