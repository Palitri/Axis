//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDirect3D11ConstantBuffer.h"

AxDirect3D11ConstantBuffer::AxDirect3D11ConstantBuffer(void)
{
	this->data = 0;
	this->dataSize = 0;
}


AxDirect3D11ConstantBuffer::~AxDirect3D11ConstantBuffer(void)
{
	this->SetDataSize(0);
}

void AxDirect3D11ConstantBuffer::SetDataSize(int dataSizeInBytes)
{
	if (this->data != 0)
	{
		delete[] this->data;
		this->data = 0;
		this->dataSize = 0;
	}

	if (dataSizeInBytes > 0)
	{
		this->dataSize = dataSizeInBytes;
		this->data = new char[this->dataSize];
		AxMem::Zero(this->data, this->dataSize);
	}
}