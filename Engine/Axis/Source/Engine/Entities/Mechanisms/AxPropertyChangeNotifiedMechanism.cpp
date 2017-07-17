//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPropertyChangeNotifiedMechanism.h"

#include "..\..\..\Tools\AxMath.h"

AxPropertyChangeNotifiedMechanism::AxPropertyChangeNotifiedMechanism(void)
	: AxMechanism()
{
	this->typeId = AxPropertyChangeNotifiedMechanism::classId;
}


AxPropertyChangeNotifiedMechanism::~AxPropertyChangeNotifiedMechanism(void)
{
}

bool AxPropertyChangeNotifiedMechanism::Process(float deltaTime)
{
	this->NotifyPropertiesChange();

	return true;
}
