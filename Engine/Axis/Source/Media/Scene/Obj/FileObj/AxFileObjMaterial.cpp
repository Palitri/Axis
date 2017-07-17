//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFileObjMaterial.h"


AxFileObjMaterial::AxFileObjMaterial(void)
{
	this->ambientMap = 0;
	this->diffuseMap = 0;
	this->specularColorMap = 0;
	this->specularHighlightMap = 0;
	this->alphaMap = 0;
	this->bumpMap = 0;
	this->displacementMap = 0;
	this->stencilMap = 0;
	this->reflectionMap = 0;
	
	this->ambientColor.x = 0.0f;
	this->ambientColor.y = 0.0f;
	this->ambientColor.z = 0.0f;

	this->diffuseColor.x = 0.0f;
	this->diffuseColor.y = 0.0f;
	this->diffuseColor.z = 0.0f;

	this->specularColor.x = 0.0f;
	this->specularColor.y = 0.0f;
	this->specularColor.z = 0.0f;
	
	this->specularFactor = 0.0f;
	this->transparency = 0.0f;
}


AxFileObjMaterial::~AxFileObjMaterial(void)
{
	if (this->ambientMap != 0)
		delete this->ambientMap;
	if (this->diffuseMap != 0)
		delete this->diffuseMap;
	if (this->specularColorMap != 0)
		delete this->specularColorMap;
	if (this->specularHighlightMap != 0)
		delete this->specularHighlightMap;
	if (this->alphaMap != 0)
		delete this->alphaMap;
	if (this->bumpMap != 0)
		delete this->bumpMap;
	if (this->displacementMap != 0)
		delete this->displacementMap;
	if (this->stencilMap != 0)
		delete this->stencilMap;
	if (this->reflectionMap != 0)
		delete this->reflectionMap;
}
