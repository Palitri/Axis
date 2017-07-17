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
}

int AxDynamicShaderBlockDefinitions::GetDefinitionIndex(AxString name)
{
	if (!name.IsEmpty())
		for (int i = 0; i < this->definitions.count; i++)
			if (this->definitions.Get(i).name.Equals(name))
				return i;

	return -1;
}

bool AxDynamicShaderBlockDefinitions::ProvideDefinition(AxString name, AxString code)
{
	return this->ProvideDefinition(name, code, AxParameterType_None);
}

bool AxDynamicShaderBlockDefinitions::ProvideDefinition(AxString name, AxString code, AxParameterType type)
{
	if (!name.IsEmpty())
        for (int i = 0; i < this->definitions.count; i++)
            if (this->definitions.Get(i).name.Equals(name))
                return false;

    AxDynamicShaderDefinition definition;
    definition.name = name;
    definition.code = code;
	definition.type = type;
    this->definitions.Add(definition);

    return true;
}

AxString AxDynamicShaderBlockDefinitions::GetCode()
{
    AxString result;

    for (int i = 0; i < this->definitions.count; i++)
    {
        result.Insert(this->definitions.Get(i).code);
        result.Insert(AxString("\r\n"));
    }

    return AxString(result);
}
