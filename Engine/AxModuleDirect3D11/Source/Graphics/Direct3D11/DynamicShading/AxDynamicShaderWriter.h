#pragma once

#include "..\..\..\AxisEngine.h"

#include "AxDynamicShaderBlockDefinitions.h"
#include "AxDynamicShaderConstantBufferInfo.h"

class AxDynamicShaderWriter
{
private:
	AxList<AxDynamicShaderConstantBufferInfo*> *constantBuffersInfo;
	AxStrings *textureBuffersInfo;

	AxDynamicShaderBlockDefinitions various, parameters, vsInputType, vsCode, sIOType, psCode, psOutputType;
    int textureSlots, texCoordSlots, numLights;

	bool animated;

	AxList<AxShadingOperation> shadingOps;

	
	AxString ProvideParameterDefinition(AxShadingOperation shadingOp, const AxString &desiredName, AxParameterType type, int elementsCount = 1, bool isArray = false, int offset = 0);
	void ProvideColorOperationCode(AxShadingOperation shadingOp, AxColorOperation colorOp, const AxString &desiredFactorName, AxDynamicShaderBlockDefinitions &definitions, AxString linePrefix, AxString destArg, AxString sourceArg);

public:
	static AxString PropertyNameToParameterName(AxString propertyName);

	AxDynamicShaderWriter(bool animated, AxList<AxDynamicShaderConstantBufferInfo*> *constantBuffersInfo, AxStrings *textureBuffersInfo);
	~AxDynamicShaderWriter(void);

	void AddShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp);
	AxString GetSourceCode();
};

