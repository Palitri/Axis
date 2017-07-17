//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDynamicShaderBlockDefinitions.h"


AxDynamicShaderBlockDefinitions::AxDynamicShaderBlockDefinitions(void)
{
}


AxDynamicShaderBlockDefinitions::~AxDynamicShaderBlockDefinitions(void)
{
	for (int i = 0; i < this->definitions.count; i++)
		delete this->definitions[i];
}

bool AxDynamicShaderBlockDefinitions::ProvideDefinition(AxString name, AxString code)
{
	if (name.length != 0)
		for (int i = 0; i < this->definitions.count; i++)
			if (this->definitions[i]->name.Equals(name))
				return false;

	AxDynamicShaderDefinition *definition = new AxDynamicShaderDefinition();
	definition->name = name;
	definition->code = code;
	this->definitions.Add(definition);

	return true;
}

AxString AxDynamicShaderBlockDefinitions::GetCode()
{
	AxString result;
	
	for (int i = 0; i < this->definitions.count; i++)
		result += this->definitions[i]->code + "\r\n";

	return AxString(result);
}
