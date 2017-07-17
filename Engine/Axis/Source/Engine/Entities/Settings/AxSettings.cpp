//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSettings.h"


AxSettings::AxSettings(void)
	: AxResource()
{
	this->typeId = AxSettings::classId;

	this->resourceType = AxResourceType_Settings;

    for (int i = 0; i < this->properties.count; i++)
		delete this->properties[i];
	this->properties.Clear();

	this->properties.Add(new AxProperty(AxString("Clear screen"), true));
	this->properties.Add(new AxProperty(AxString("Clear screen color"), AxVector4(0.84f, 0.92f, 1.0f, 1.0f)));
	this->properties.Add(new AxProperty(AxString("Ambient light"), AxVector4(0.25f, 0.25f, 0.25f, 1.0f)));
	this->properties.Add(new AxProperty(AxString("Thumbnail"), true));
	this->properties.Add(new AxProperty(AxString("Thumbnail size"), 256));
	this->properties.Add(new AxProperty(AxString("Embed generated meshes"), true));
	this->properties.Add(new AxProperty(AxString("Embed generated textures"), false));
	this->properties.Add(new AxProperty(AxString("Embed imported textures"), false));
	this->properties.Add(new AxProperty(AxString("Embedded textures original encoding"), true));
}


AxSettings::~AxSettings(void)
{
}
