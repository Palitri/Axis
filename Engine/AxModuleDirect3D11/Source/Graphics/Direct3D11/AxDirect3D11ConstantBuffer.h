#pragma once

#include "..\..\AxisEngine.h"

#include "AxDirect3D11GraphicsDevice.h"

class AxDirect3D11ConstantBuffer
{
public:
	ID3D11Buffer *buffer;

	void *data;
	int dataSize;

	AxDirect3D11ConstantBuffer(void);
	~AxDirect3D11ConstantBuffer(void);

	void SetDataSize(int dataSizeInBytes);
};

