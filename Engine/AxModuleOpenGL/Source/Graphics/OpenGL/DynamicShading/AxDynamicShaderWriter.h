#pragma once

#include "..\..\..\AxisEngine.h"

#include "AxDynamicShaderBlockDefinitions.h"

class AxDynamicShaderWriter
{
public:
	static AxString PropertyNameToParameterName(AxString propertyName);

	AxDynamicShaderBlockDefinitions *vsInputType, *vsParams, *vsCode, *sIOType, *psParams, *psCode, *psOutputType;

	AxList<AxShadingOperation> shadingOps;

	bool animated;
	int maxBones;

	int numLights, textureSlots, texCoordSlots;

	AxDynamicShaderWriter(bool animated, int maxBones);
	~AxDynamicShaderWriter(void);

	void AddShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp);

	AxString GetVSSourceCode();
	AxString GetPSSourceCode();
	AxString ProvideParameterDefinition(AxDynamicShaderBlockDefinitions *definitions, AxShadingOperation shadingOp, AxString desiredName, AxParameterType type, int elementsCount = 1, bool isArray = false);
	void ProvideColorOperationCode(AxShadingOperation shadingOp, AxColorOperation colorOp, AxString desiredFactorName, AxDynamicShaderBlockDefinitions *definitions, AxString linePrefix, AxString destArg, AxString sourceArg);
	
	static AxString TextureSampling(AxString texture, AxString texCoords);
	static AxString TextureSampling(AxString texture, AxString texCoords, AxString texCoords1);
};

