//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDynamicShaderConstantBufferInfo.h"

int AxDynamicShaderConstantInfo::GetAddress(int index)
{
	return this->offset + index * this->elementPitch;
}


AxDynamicShaderConstantBufferInfo::AxDynamicShaderConstantBufferInfo(void)
{
	this->totalSize = 0;
}


AxDynamicShaderConstantBufferInfo::~AxDynamicShaderConstantBufferInfo(void)
{
	this->Clear();
}

// Constants in the HLSL buffer are packed, so that more than one constant my actually inhabit the same float4 vector (if they can fit in that vector).
// A constant is placed in a new vector (thus possibly creating padding) if it can't fit in the last used vector (if for example some part of the vector is already taken by other constants)
// Structures and arrays are automatically placed to start from a new vector
// Arrays are not packed. Each element of the array is stored in a float4 vector
bool AxDynamicShaderConstantBufferInfo::Add(AxString name, int elementSize, int elementCount, int offset, bool isArray)
{
	if (elementCount > 1)
		isArray = true;

	int elementPitch = AxMaths::GetDivisibleBy(elementSize, AxDynamicShaderConstantBufferInfo::VectorSize);

	int size;
	if (isArray)
	{
		this->totalSize = AxMaths::GetDivisibleBy(this->totalSize, AxDynamicShaderConstantBufferInfo::VectorSize);
		size = elementCount * elementPitch;
	}
	else
	{
		int vectorOffset = this->totalSize % AxDynamicShaderConstantBufferInfo::VectorSize;
		if ((vectorOffset != 0) && (vectorOffset + elementSize > AxDynamicShaderConstantBufferInfo::VectorSize))
			this->totalSize = AxMaths::GetDivisibleBy(this->totalSize, AxDynamicShaderConstantBufferInfo::VectorSize);
		size = elementCount * elementSize;
	}

	if (this->totalSize + size > AxDynamicShaderConstantBufferInfo::MaxSize)
		return false;

	AxDynamicShaderConstantInfo constantInfo;
	constantInfo.name = name;
	constantInfo.offset = this->totalSize + offset;
	constantInfo.elementSize = elementSize;
	constantInfo.elementCount = elementCount;
	constantInfo.elementPitch = elementPitch;

	if (offset == 0)
		this->totalSize += size;

	AxList<AxDynamicShaderConstantInfo>::Add(constantInfo);

	return true;
}


void AxDynamicShaderConstantBufferInfo::Clear()
{
	this->totalSize = 0;

	AxList<AxDynamicShaderConstantInfo>::Clear();
}
