#pragma once

#include "..\..\..\AxisEngine.h"

class AxDynamicShaderDefinition
{
public:
	AxString name, code;
	AxParameterType type;
};

class AxDynamicShaderBlockDefinitions
{
public:
	AxList<AxDynamicShaderDefinition> definitions;

	AxDynamicShaderBlockDefinitions(void);
	~AxDynamicShaderBlockDefinitions(void);

	int GetDefinitionIndex(AxString name);
	bool ProvideDefinition(AxString name, AxString code);
	bool ProvideDefinition(AxString name, AxString code, AxParameterType type);
	AxString GetCode();
};

