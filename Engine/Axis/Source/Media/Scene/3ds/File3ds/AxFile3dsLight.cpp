//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFile3dsLight.h"


AxFile3dsLight::AxFile3dsLight(void)
{
	this->objectType = AxFile3dsObjectType_Light;

	this->active = true;

	this->color.r = 1.0f;
	this->color.g = 1.0f;
	this->color.b = 1.0f;
	
	this->position.x = 0.0f;
	this->position.y = 0.0f;
	this->position.z = 0.0f;

	this->spotLight = false;
	this->spotlight_Target.x = 0.0f;
	this->spotlight_Target.y = 0.0f;
	this->spotlight_Target.z = 0.0f;

	this->spotlight_Hotspot = 0.0f;
	this->spotlight_Falloff = 0.0f;
}


AxFile3dsLight::~AxFile3dsLight(void)
{
}
