//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxHueMapTextureMechanism.h"

#include "..\..\Textures\AxTexture2D.h"

#include "..\..\..\..\Media\Image\AxImage.h"

#include "..\..\..\Utilities\Imaging\AxHueMapImage.h"

AxHueMapTextureMechanism::AxHueMapTextureMechanism(void)
{
	this->typeId = AxHueMapTextureMechanism::classId;

	this->properties.Add(new AxProperty("Width", 256));
	this->properties.Add(new AxProperty("Height", 256));
	this->properties.Add(new AxProperty("Lightness", 1.0f));
    this->properties.Add(new AxProperty("Texture", 0, AxParameterType_ReferenceTexture));
}


AxHueMapTextureMechanism::~AxHueMapTextureMechanism(void)
{
}

void AxHueMapTextureMechanism::OnPropertiesChanged()
{
	int width = this->properties[AxHueMapTextureMechanism::propertyIndex_Width]->GetInt();
	int height = this->properties[AxHueMapTextureMechanism::propertyIndex_Height]->GetInt();
	float lightness = this->properties[AxHueMapTextureMechanism::propertyIndex_Lightness]->GetFloat();
	AxTexture2D *texture = (AxTexture2D*)this->properties[AxHueMapTextureMechanism::propertyIndex_Texture]->GetEffectiveValue();

	if (texture == 0)
		return;

	AxImage image;
	AxHueMapImage(image, width, height, AxVector4(0.0f, 1.0f, 0.0f, 1.0f), AxVector4(1.0f, 1.0f, 0.0f, 1.0f), AxVector4(0.0f, 0.0f, lightness, 0.0f), AxVector4(0.0f, 0.0f, 0.5f, 0.0f), false, true);
	texture->Load(image);
}