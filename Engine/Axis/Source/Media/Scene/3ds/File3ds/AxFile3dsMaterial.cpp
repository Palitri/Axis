//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFile3dsMaterial.h"


AxFile3dsMaterial::AxFile3dsMaterial(void)
{
	this->ambient.r = 0x00;
	this->ambient.g = 0x00;
	this->ambient.b = 0x00;

	this->diffuse.r = 0x00;
	this->diffuse.g = 0x00;
	this->diffuse.b = 0x00;

	this->specular.r = 0x00;
	this->specular.g = 0x00;
	this->specular.b = 0x00;
	
	this->specularLevel = 0.0f;
	this->glossiness = 0.0f;
	
	this->hasAmbient = false;
	this->hasDiffuse = false;
	this->hasSpecular = false;
}


AxFile3dsMaterial::~AxFile3dsMaterial(void)
{
}
