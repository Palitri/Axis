//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxParameter.h"

#include "AxParameterType.h"

#include "..\Entities\Mechanisms\AxMechanism.h"

#include "..\Primitive\AxVector3.h"
#include "..\Primitive\AxVector4.h"

AxParameter *AxParameter::GetEffectiveParameter(AxParameter *parameter)
{
    // TODO: Parameters shouldn't be (so directly) dependent on mechanisms, since mechanisms are considered a higher-level objects
	if (parameter->type == AxParameterType_ReferenceMechanism)
    {
        return AxParameter::GetEffectiveParameter(&((AxMechanism*)parameter->value)->parameter);
    }
    else
        return parameter;
}

bool AxParameter::IsReference(AxParameterType type)
{
	return type >= 1000;
}

AxParameter::AxParameter(void)
{
	this->type = AxParameterType_None;
	this->value = 0;
}

AxParameter::AxParameter(int value)
{
	this->type = AxParameterType_None;
	this->value = 0;

	this->SetInt(value);
}

AxParameter::AxParameter(float value)
{
	this->type = AxParameterType_None;
	this->value = 0;

	this->SetFloat(value);
}

AxParameter::AxParameter(bool value)
{
	this->type = AxParameterType_None;
	this->value = 0;

	this->SetBool(value);
}

AxParameter::AxParameter(const AxVector4 &value)
{
	this->type = AxParameterType_None;
	this->value = 0;

	this->SetColor(value);
}

AxParameter::AxParameter(void *value, AxParameterType type)
{
	this->type = AxParameterType_None;
	this->value = 0;

	this->SetValue(value, type);
}
    
AxParameter::~AxParameter(void)
{
	this->DisposeValue();
}

bool AxParameter::Equals(AxParameter &parameter)
{
	if (this->type != parameter.type)
		return false;

	switch (this->type)
	{
		case AxParameterType_Int:
			return *(int*)this->value == *(int*)parameter.value;
		case AxParameterType_Float:
			return *(float*)this->value == *(float*)parameter.value;
		case AxParameterType_Bool:
			return *(bool*)this->value == *(bool*)parameter.value;
		case AxParameterType_Color:
			return ((AxVector4*)this->value)->Equals(*((AxVector4*)parameter.value));
	}

	return this->value == parameter.value;
}

bool AxParameter::IsReference()
{
	return AxParameter::IsReference(this->type);
}

void AxParameter::DisposeValue()
{
	if (this->value == 0)
		return;

	switch (this->type)
	{
		case AxParameterType_Int:
		case AxParameterType_Float:
		case AxParameterType_Bool:
		case AxParameterType_Color:
		{
			// Only non-reference types have allocated memory and are to be deleted
			delete this->value;
			break;
		}
	}

	this->value = 0;
}

AxParameter *AxParameter::GetEffectiveParameter()
{
    return AxParameter::GetEffectiveParameter(this);
}

void AxParameter::RecallEffectiveParameter()
{
	AxParameter *parameter = this->GetEffectiveParameter();

	if (parameter != this)
		this->SetValue(parameter->value, parameter->type);
}

void AxParameter::SetType(AxParameterType type)
{
	if (this->type == type)
		return;

	this->DisposeValue();

	switch (type)
	{
		case AxParameterType_Int:
		{
			this->value = new int();
			break;
		}

		case AxParameterType_Float:
		{
			this->value = new float();
			break;
		}

		case AxParameterType_Bool:
		{
			this->value = new bool();
			break;
		}

		case AxParameterType_Color:
		{
			this->value = new AxVector4();
			break;
		}
	}

	this->type = type;
}


void AxParameter::SetValue(void *value, AxParameterType type)
{
	this->SetType(type);
	
	switch (type)
	{
		case AxParameterType_Int:
		{
			*(int*)this->value = *(int*)value;
			break;
		}

		case AxParameterType_Float:
		{
			*(float*)this->value = *(float*)value;
			break;
		}

		case AxParameterType_Bool:
		{
			*(bool*)this->value = *(bool*)value;
			break;
		}

		case AxParameterType_Color:
		{
			*(AxVector4*)this->value = *(AxVector4*)value;
			break;
		}

		default:
		{
			this->value = value;
		}
	}
}

void *AxParameter::GetEffectiveValue()
{
	return this->GetEffectiveParameter()->value;
}

void AxParameter::SetInt(int value)
{
	this->SetType(AxParameterType_Int);
	*(int*)this->value = value;
}

int AxParameter::GetInt()
{
	return *(int*)this->GetEffectiveParameter()->value;
}

void AxParameter::SetFloat(float value)
{
	this->SetValue(&value, AxParameterType_Float);
}

float AxParameter::GetFloat()
{
	return *(float*)this->GetEffectiveParameter()->value;
}

void AxParameter::SetBool(bool value)
{
	this->SetValue(&value, AxParameterType_Bool);
}

bool AxParameter::GetBool()
{
	return *(bool*)this->GetEffectiveParameter()->value;
}

void AxParameter::SetColor(const AxVector4 &value)
{
	AxVector4 v(value.x, value.y, value.z, value.w);
	this->SetValue(&v, AxParameterType_Color);
}

AxVector4 AxParameter::GetColor()
{
	return *(AxVector4*)this->GetEffectiveParameter()->value;
}