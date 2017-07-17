#pragma once

#include "..\DeviceIndependent\AxDeviceIndependentShader.h"

#include "Engine\Soras.h"

#include "..\..\Engine\Primitive\AxVector4.h"
#include "..\..\Engine\Primitive\AxMatrix.h"

class AxSorasGraphicsDevice;

class AxSorasShader :
	public AxDeviceIndependentShader
{
private:
	static const int paramAddress_Ambient = 0;
	static const int paramAddress_Diffuse = 1;
	static const int paramAddress_Specular = 2;
	static const int paramAddress_Gloss = 3;
	static const int paramAddress_ColorMap = 4;
	static const int paramAddress_ReflectionMap = 5;
	static const int paramAddress_NormalMap = 6;
	static const int paramAddress_RefractionMap = 7;
	static const int paramAddress_ActiveLightsCount = 8;
	static const int paramAddress_ViewMatrix = 9;
	static const int paramAddress_ProjectionMatrix = 10;
	static const int paramAddress_WorldMatrix = 11;
	static const int paramAddress_LightsPos = 12;
	static const int paramAddress_LightsColor = 13;
	static const int paramAddress_LightsIntensity = 14;
	static const int paramAddress_AmbientLight = 15;
	static const int paramAddress_FogColor = 16;
	static const int paramAddress_FogStart = 17;
	static const int paramAddress_FogDensity = 18;
	static const int paramAddress_TextureTransform = 19;

	// Indices to pixelOpsTextures, -1 if not used.
	int colorMapIndex, envMapIndex, normalMapIndex, refractionMapIndex;

	// Soras has fogStart and fogEnd, while Axis has fogStart and fogDensity, so if Axis.fogDensity comes before Axis.fogStart, there's no way to calculate Soras.fogEnd, that's why we keep it and calculate Soras.fogEnd at both Axis.fogStart and Axis.fogDensity
	float fogDensity;

	void AxToSrARGBInt(SrARGBInt &result, AxVector4 &source);
	void AxToSrARGB(SrARGB &result, AxVector4 &source);

public:
	AxSorasGraphicsDevice *context;

	bool isAnimated;
	AxMatrix viewMatrix, projectionMatrix;

	AxList<SrShaderOperation> vertexShaderOps, vertexColorOps, pixelShaderOps, pixelColorOps;
	AxList<SrTexture*> pixelOpsTextures;

	AxSorasShader(void);
	virtual ~AxSorasShader(void);

	virtual int GetParameterAddress(AxString parameterName);
	virtual void SetParameter(int address, int index, void *value);

	virtual bool Update(AxString &compilationMessage);
};

