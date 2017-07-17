//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPolynomialMechanism.h"

#include "..\..\..\Tools\AxMath.h"


AxPolynomialMechanism::AxPolynomialMechanism(int degree)
	: AxMechanism()
{
	this->typeId = AxPolynomialMechanism::classId;

	this->parameter.SetFloat(0.0f);

	this->properties.Add(new AxProperty("x", 0.0f));

	this->degree = -1;
	this->SetDegree(degree);

	if (this->degree >= 1)
		this->properties.Get(AxPolynomialMechanism::GetDegreePropertyIndex(1))->SetFloat(1.0f);
}


AxPolynomialMechanism::~AxPolynomialMechanism(void)
{
}

bool AxPolynomialMechanism::Process(float deltaTime)
{
	float variable =  *(float*)this->properties[AxPolynomialMechanism::propertyIndex_X]->GetEffectiveParameter()->value;
	float result = 0.0f;

	for (int i = 0; i <= AxPolynomialMechanism::degree; i++)
		result += (*(float*)this->properties.Get(AxPolynomialMechanism::GetDegreePropertyIndex(i))->GetEffectiveValue()) * AxMath::Power(variable, (float)i);

	*(float*)this->parameter.value = result;

    return true;
}

void AxPolynomialMechanism::SetDegree(int degree)
{
	// Reduce degree
	for (int i = degree + 1; i <= this->degree; i++)
	{
		int index = AxPolynomialMechanism::GetDegreePropertyIndex(i);
		delete this->properties.Get(index);
		this->properties.RemoveAt(index);
	}

	// Raise degree
	for (int i = this->degree + 1; i <= degree; i++)
		this->properties.Insert(AxPolynomialMechanism::GetDegreePropertyIndex(i), new AxProperty(AxString("x") + AxString(i), 0.0f));

	this->degree = degree;
}

void AxPolynomialMechanism::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	writer.BeginChunk(AxPolynomialMechanism::SerializationId_Degree);
	writer.stream->WriteInt32(this->degree);
	writer.EndChunk();

	AxMechanism::SerializeChunks(writer);
}

bool AxPolynomialMechanism::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxMechanism::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxPolynomialMechanism::SerializationId_Degree:
		{
			this->SetDegree(reader.stream->ReadInt32());
			break;
		}
	}

	return true;
}



// Static methods



int AxPolynomialMechanism::GetDegreePropertyIndex(int degree)
{
	return AxPolynomialMechanism::propertyIndex_Degree0 + degree;
}
