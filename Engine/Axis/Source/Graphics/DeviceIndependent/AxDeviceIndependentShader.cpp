//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDeviceIndependentShader.h"


AxDeviceIndependentShader::AxDeviceIndependentShader(void)
{
}


AxDeviceIndependentShader::~AxDeviceIndependentShader(void)
{
}

int AxDeviceIndependentShader::GetOperationCount()
{
	return this->shadingOps.count;
}

void AxDeviceIndependentShader::AddOperation(AxShadingOperation shadingOp, AxColorOperation colorOp)
{
	AxDeviceIndependentMaterialShadingOperation shadingOperation;
	shadingOperation.shadingOp = shadingOp;
	shadingOperation.colorOp = colorOp;

	this->shadingOps.Add(shadingOperation);
}

void AxDeviceIndependentShader::GetOperation(int index, AxShadingOperation &shadingOp, AxColorOperation &colorOp)
{
	shadingOp = this->shadingOps[index].shadingOp;
	colorOp = this->shadingOps[index].colorOp;
}

void AxDeviceIndependentShader::SetOperation(int index, AxShadingOperation shadingOp, AxColorOperation colorOp)
{
	this->shadingOps[index].shadingOp = shadingOp;
	this->shadingOps[index].colorOp = colorOp;
}

void AxDeviceIndependentShader::ClearOperations()
{
	this->shadingOps.Clear();
}

int AxDeviceIndependentShader::GetParameterAddress(AxString parameterName)
{
	return -1;
}

void AxDeviceIndependentShader::SetParameter(int address, void *value)
{
}

bool AxDeviceIndependentShader::Update()
{
	return true;
}

int AxDeviceIndependentShader::IndexOf(AxShadingOperation shadingOp)
{
	for (int i = 0; i < this->shadingOps.count; i++)
		if (this->shadingOps[i].shadingOp == shadingOp)
			return i;

	return -1;
}
