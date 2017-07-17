//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPlasmaFractalTextureMechanism.h"

#include "..\..\Textures\AxTexture2D.h"

#include "..\..\..\..\Media\Image\AxImage.h"

#include "..\..\..\Utilities\Imaging\AxPlasmaFractalImage.h"


AxPlasmaFractalTextureMechanism::AxPlasmaFractalTextureMechanism(void)
{
	this->typeId = AxPlasmaFractalTextureMechanism::classId;

	this->properties.Add(new AxProperty("Size", 256));
	this->properties.Add(new AxProperty("Color", AxVector4(1.0f)));
	this->properties.Add(new AxProperty("Roughness", 0.5f));
    this->properties.Add(new AxProperty("Texture", 0, AxParameterType_ReferenceTexture));
}


AxPlasmaFractalTextureMechanism::~AxPlasmaFractalTextureMechanism(void)
{
}

void AxPlasmaFractalTextureMechanism::OnPropertiesChanged()
{
	int size = this->properties[AxPlasmaFractalTextureMechanism::propertyIndex_Size]->GetInt();
	AxVector4 color = this->properties[AxPlasmaFractalTextureMechanism::propertyIndex_Color]->GetColor();
	float roughness = this->properties[AxPlasmaFractalTextureMechanism::propertyIndex_Roughness]->GetFloat();
	AxTexture2D *texture = (AxTexture2D*)this->properties[AxPlasmaFractalTextureMechanism::propertyIndex_Texture]->GetEffectiveValue();

	if (texture == 0)
		return;

	AxImage image;
	AxPlasmaFractalImage(image, size, color, roughness);
	texture->Load(image);
}