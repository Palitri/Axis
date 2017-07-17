#pragma once

#include "..\..\AxisEngine.h"

#include "AxDirect3D11GraphicsDevice.h"
#include "DynamicShading\AxDynamicShaderConstantBufferInfo.h"

#include "AxDirect3D11ConstantBuffer.h"

class AxDirect3D11Shader :
	public AxDeviceIndependentShader
{
private:
	static const int parameterAddress_Invalid			= -1;
	static const int parameterAddress_ViewMatrix		= -2;
	static const int parameterAddress_ProjectionMatrix	= -3;

	AxString compilationErrorMessage;


	bool CompileShader(AxString &shaderCode);
	bool CompileShaderFunction(AxString &shaderCode, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	void Release();

public:
	AxList<AxDynamicShaderConstantBufferInfo*> constantBuffersInfo;
	AxStrings textureBuffersInfo;

	AxMatrix viewMatrix, projectionMatrix;

	AxDirect3D11GraphicsDevice *context;

	ID3D11VertexShader*     vertexShader;
	ID3D11PixelShader*      pixelShader;
	
	ID3D11InputLayout*      vertexLayout;

	AxList<AxDirect3D11ConstantBuffer> constantBuffers;
	AxList<ID3D11ShaderResourceView*> shaderTexturesBuffer;
	AxList<ID3D11SamplerState*> shaderSamplersBuffer;

	AxDirect3D11Shader(void);
	~AxDirect3D11Shader(void);

	int GetParameterAddress(AxString parameterName);
	void SetParameter(int address, int index, void *value);

	bool Update(AxString &compilationMessage);
};

