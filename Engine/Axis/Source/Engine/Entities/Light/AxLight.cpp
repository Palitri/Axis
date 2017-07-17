//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxLight.h"

#include "..\..\Primitive\AxVector4.h"

AxLight::AxLight(void)
	: AxResource()
{
	this->typeId = AxLight::classId;
	this->resourceType = AxResourceType_Light;
       
    this->properties.Add(new AxProperty(AxString("Color"), AxVector4(1.0f)));
    this->properties.Add(new AxProperty(AxString("Intensity"), 1000.0f));
}

AxLight::~AxLight(void)
{
}
