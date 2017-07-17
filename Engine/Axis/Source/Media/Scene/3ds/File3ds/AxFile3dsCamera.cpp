//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFile3dsCamera.h"


AxFile3dsCamera::AxFile3dsCamera(void)
{
	this->objectType = AxFile3dsObjectType_Camera;

	this->position.x = 0.0f;
	this->position.y = 0.0f;
	this->position.z = 0.0f;
	
	this->target.x = 0.0f;
	this->target.y = 0.0f;
	this->target.z = 0.0f;

	this->bank = 0.0f;
	this->lens = 0.0f;
}


AxFile3dsCamera::~AxFile3dsCamera(void)
{
}
