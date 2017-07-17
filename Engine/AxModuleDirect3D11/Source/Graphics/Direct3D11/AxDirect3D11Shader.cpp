//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDirect3D11Shader.h"

#include <Direct3D11\D3Dcompiler.h>

#include "DynamicShading\AxDynamicShaderWriter.h"

#include "AxDirect3D11Texture2D.h"

AxDirect3D11Shader::AxDirect3D11Shader(void)
{
	this->vertexShader = 0;
	this->pixelShader = 0;

	this->vertexLayout = 0;
}


AxDirect3D11Shader::~AxDirect3D11Shader(void)
{
	this->Release();
}

int AxDirect3D11Shader::GetParameterAddress(AxString parameterName)
{
	
	if (parameterName.Equals("View matrix"))
		return AxDirect3D11Shader::parameterAddress_ViewMatrix;
	if (parameterName.Equals("Projection matrix"))
		return AxDirect3D11Shader::parameterAddress_ProjectionMatrix;

	AxString shaderParameterName = AxDynamicShaderWriter::PropertyNameToParameterName(parameterName);

	for (int cBufferIndex = 0; cBufferIndex < this->constantBuffersInfo.count; cBufferIndex++)
	{
		AxList<AxDynamicShaderConstantInfo> *cbufferInfo = this->constantBuffersInfo[cBufferIndex];
		for (int cIndex = 0; cIndex < cbufferInfo->count; cIndex++)
			if (cbufferInfo->Get(cIndex).name.Equals(shaderParameterName))
				return (cBufferIndex << 16) | cIndex;
	}

	for (int tBufferIndex = 0; tBufferIndex < this->textureBuffersInfo.count; tBufferIndex++)
		if (this->textureBuffersInfo[tBufferIndex].Equals(shaderParameterName))
			return (this->constantBuffersInfo.count + tBufferIndex) << 16;

	return AxDirect3D11Shader::parameterAddress_Invalid;
}

void AxDirect3D11Shader::SetParameter(int address, int index, void *value)
{
	if (address == AxDirect3D11Shader::parameterAddress_ViewMatrix)
		this->viewMatrix = *(AxMatrix*)(value);
	if (address == AxDirect3D11Shader::parameterAddress_ProjectionMatrix)
		this->projectionMatrix = *(AxMatrix*)(value);

	if (address < 0)
		return;

	int bufferIndex = address >> 16;
	int parameterIndex = address & 0xffff;

	if (bufferIndex < this->constantBuffersInfo.count)
	{
		if (value == 0)
			return;

		AxDynamicShaderConstantInfo *constantInfo = &this->constantBuffersInfo[bufferIndex]->Get(parameterIndex);
		int offset = constantInfo->GetAddress(index);
		AxMem::Copy((char*)this->constantBuffers[bufferIndex].data + offset, value, constantInfo->elementSize);
	}
	else
	{
		bufferIndex -= this->constantBuffersInfo.count;
		if (bufferIndex < this->textureBuffersInfo.count)
		{
			AxDirect3D11Texture2D *texture = (AxDirect3D11Texture2D*)value;

			if (this->shaderTexturesBuffer.count < bufferIndex + 1)
				this->shaderTexturesBuffer.SetSize(bufferIndex + 1);
			this->shaderTexturesBuffer.Set(bufferIndex, texture == 0 ? NULL : texture->deviceShaderResourceView);

			if (this->shaderSamplersBuffer.count < bufferIndex + 1)
				this->shaderSamplersBuffer.SetSize(bufferIndex + 1);
			this->shaderSamplersBuffer.Set(bufferIndex, texture == 0 ? NULL : texture->deviceSampler);
		}
	}
}

bool AxDirect3D11Shader::Update(AxString &compilationMessage)
{
	this->Release();

	bool isAnimated = this->IndexOf(AxShadingOperation_Skinning) != -1;
	AxDynamicShaderWriter shaderWriter(isAnimated, &this->constantBuffersInfo, &this->textureBuffersInfo);

	for (int i = 0; i < this->shadingOps.count; i++)
		shaderWriter.AddShadingLayer(this->shadingOps[i].shadingOp, this->shadingOps[i].colorOp);

	AxString shaderCode = shaderWriter.GetSourceCode();
	//AxFileStream(L"LastShader.fx", FileStreamAccessMode_Write).WriteText(shaderCode);

	if (!this->CompileShader(shaderCode))
	{
		compilationMessage = this->compilationErrorMessage;
		return false;
	}

	this->constantBuffers.SetSize(this->constantBuffersInfo.count);
	for (int constantBufferIndex = 0; constantBufferIndex < this->constantBuffersInfo.count; constantBufferIndex++)
	{
		AxDirect3D11ConstantBuffer *constantBuffer = &this->constantBuffers[constantBufferIndex];
		
		// Create a data buffer, which will be used to store constants into, and have its whole contents instantly to the constant buffer of the device, when rendering with the shader
		// Constant buffer size must be a multiple of AxDynamicShaderConstantBufferInfo::VectorSize (the size of a float4 = 16)
		constantBuffer->SetDataSize(AxMaths::GetDivisibleBy(this->constantBuffersInfo[constantBufferIndex]->totalSize, AxDynamicShaderConstantBufferInfo::VectorSize));

		// Create the constant buffer
		D3D11_BUFFER_DESC bufferDescription;
		AxMem::Zero(&bufferDescription, sizeof(bufferDescription));
		bufferDescription.Usage = D3D11_USAGE_DEFAULT;
		bufferDescription.ByteWidth = constantBuffer->dataSize;
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = 0;
		if (FAILED(this->context->device->CreateBuffer(&bufferDescription, NULL, &constantBuffer->buffer)))
			return false;
	}

	return true;
}

bool AxDirect3D11Shader::CompileShader(AxString &shaderCode)
{
	// Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
	if (!this->CompileShaderFunction(shaderCode, "VSMain", "vs_4_0", &pVSBlob))
        return false;

	HRESULT hr;
	hr = this->context->device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &this->vertexShader);
	// Create the vertex shader
	if (FAILED(hr))
	{	
		pVSBlob->Release();
        return false;
	}

    // Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,								D3D11_INPUT_PER_VERTEX_DATA,	0 },
        { "NORMAL",			0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	3 * 4,							D3D11_INPUT_PER_VERTEX_DATA,	0 },
        { "TEXCOORD",		0,	DXGI_FORMAT_R32G32_FLOAT,		0,	(3 + 3) * 4,					D3D11_INPUT_PER_VERTEX_DATA,	0 },
		{ "BLENDINDICES",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	(3 + 3 + 2) * 4,				D3D11_INPUT_PER_VERTEX_DATA,	0 },
        { "BLENDWEIGHT",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	(3 + 3 + 2 + 4) * 4,			D3D11_INPUT_PER_VERTEX_DATA,	0 },
        { "TANGENT",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	(3 + 3 + 2 + 4 + 4) * 4,		D3D11_INPUT_PER_VERTEX_DATA,	0 },
        { "BINORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	(3 + 3 + 2 + 4 + 4 + 3) * 4,	D3D11_INPUT_PER_VERTEX_DATA,	0 },
    };
	UINT numElements = ARRAYSIZE(layout);

    // Create the input layout
	hr = this->context->device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &this->vertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
        return false;

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	if (!this->CompileShaderFunction(shaderCode, "PSMain", "ps_4_0", &pPSBlob))
        return false;

	// Create the pixel shader
	hr = this->context->device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &this->pixelShader);
	pPSBlob->Release();
    if (FAILED(hr))
        return false;

	return true;
}

bool AxDirect3D11Shader::CompileShaderFunction(AxString &shaderCode, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
	const char *c = shaderCode.GetCharContents();
	int cl = shaderCode.GetLength();

	ID3DBlob* pErrorBlob;
	if (FAILED(D3DCompile(
		shaderCode.GetCharContents(),		// HLSL code of the shader
		shaderCode.GetLength(), 			// Size of the HLSL code in bytes
		NULL,								// File name
		NULL,								// Macro definitions
		NULL,								// Pointer to the shader's include files. Should be non-NULL, if the shader contains #include
		szEntryPoint,						// Entry point function name. The shader function which is to be compiled
		szShaderModel,						// Shader model compiling profile string. Valid values are "vs_4_0", "ps_4_0" and so on.
		dwShaderFlags,						// Shader compile flags
		0,									// Effect compile flags. Should be 0 when compiling a shader and not an effect
		ppBlobOut,							// The compiled shader code
		&pErrorBlob)))						// Error msg if compilation went wrong
	{
        if (pErrorBlob != NULL)
		{
            this->compilationErrorMessage = (char*)pErrorBlob->GetBufferPointer();
			pErrorBlob->Release();
		}


		return false;
	}

    if (pErrorBlob)
		pErrorBlob->Release();

    return true;
}

void AxDirect3D11Shader::Release()
{
	for (int i = 0; i < this->constantBuffers.count; i++)
	{
		if (this->constantBuffers[i].data != 0)
		{
			delete[] this->constantBuffers[i].data;
			this->constantBuffers[i].data = 0;
		}

		if (this->constantBuffers[i].buffer != 0)
		{
			this->constantBuffers[i].buffer->Release();
			this->constantBuffers[i].buffer = 0;
		}
	}
	this->constantBuffers.Clear();

	for (int i = 0; i < this->constantBuffersInfo.count; i++)
		delete this->constantBuffersInfo[i];
	this->constantBuffersInfo.Clear();

	this->textureBuffersInfo.Clear();


	if (this->vertexShader != 0)
	{
		this->vertexShader->Release();
		this->vertexShader = 0;
	}

	if (this->pixelShader != 0)
	{
		this->pixelShader->Release();
		this->pixelShader = 0;
	}


	if (this->vertexLayout != 0)
	{
		this->vertexLayout->Release();
		this->vertexLayout = 0;
	}
}