#pragma once

#include "..\..\..\AxisEngine.h"

struct AxDynamicShaderDefinition
{
	AxString name, code;
};

class AxDynamicShaderBlockDefinitions
{
private:
	AxList<AxDynamicShaderDefinition*> definitions;

public:
	AxDynamicShaderBlockDefinitions(void);
	~AxDynamicShaderBlockDefinitions(void);

	bool ProvideDefinition(AxString name, AxString code);
	AxString GetCode();
};

