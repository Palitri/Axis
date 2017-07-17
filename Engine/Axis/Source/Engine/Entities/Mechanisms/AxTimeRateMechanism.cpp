//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTimeRateMechanism.h"

#include "..\..\Axis.h"

AxTimeRateMechanism::AxTimeRateMechanism(Axis *context)
{
	this->context = context;

	this->typeId = AxTimeRateMechanism::classId;

	this->parameter.SetFloat(this->context->timer->speed);
}


AxTimeRateMechanism::~AxTimeRateMechanism(void)
{
}

bool AxTimeRateMechanism::Process(float deltaTime)
{
	this->parameter.SetFloat(this->context->timer->speed);

	return true;
}
