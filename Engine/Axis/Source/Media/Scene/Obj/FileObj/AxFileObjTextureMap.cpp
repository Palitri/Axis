//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFileObjTextureMap.h"


AxFileObjTextureMap::AxFileObjTextureMap(void)
{
	this->offset.x = 0.0f;
	this->offset.y = 0.0f;
	this->offset.z = 0.0f;

	this->scale.x = 1.0f;
	this->scale.y = 1.0f;
	this->scale.z = 1.0f;
	
	this->turbulence.x = 0.0f;
	this->turbulence.y = 0.0f;
	this->turbulence.z = 0.0f;

	this->brightness = 0.0f;
	this->contrastFactor = 1.0;
	this->bumpFactor = 1.0f;
	this->mipMapSharpness = 0.0f;

	this->resolution = -1;
	
	this->clamp = false;
	this->blendU = true;
	this->blendV = true;

	this->mapType = AxFileObjTextureMapType_Default;
	this->mapChannel = AxFileObjTextureMapChannel_Default;
}


AxFileObjTextureMap::~AxFileObjTextureMap(void)
{
}
