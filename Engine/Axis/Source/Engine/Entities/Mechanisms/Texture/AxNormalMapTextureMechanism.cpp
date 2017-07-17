//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNormalMapTextureMechanism.h"

#include "..\..\..\Primitive\AxVector4.h"

#include "..\..\Textures\AxTexture2D.h"

#include "..\..\..\..\Media\Image\AxImage.h"

#include "..\..\..\Utilities\Imaging\AxGenerateNormalMapImage.h"


AxNormalMapTextureMechanism::AxNormalMapTextureMechanism(void)
{
	this->typeId = AxNormalMapTextureMechanism::classId;

	this->properties.Add(new AxProperty("Emphasis", 0.0f));
	this->properties.Add(new AxProperty("Filter", AxVector4(1.0f, 1.0f, 1.0f, 0.0f)));
    this->properties.Add(new AxProperty("Source height map", 0, AxParameterType_ReferenceTexture));
    this->properties.Add(new AxProperty("Texture", 0, AxParameterType_ReferenceTexture));
}


AxNormalMapTextureMechanism::~AxNormalMapTextureMechanism(void)
{
}

void AxNormalMapTextureMechanism::OnPropertiesChanged()
{
	float emphasis = this->properties[AxNormalMapTextureMechanism::propertyIndex_Emphasis]->GetFloat();
	AxVector4 filter = this->properties[AxNormalMapTextureMechanism::propertyIndex_Filter]->GetColor();
	AxTexture2D *sourceHeightMap = (AxTexture2D*)this->properties[AxNormalMapTextureMechanism::propertyIndex_SourceHeightMap]->GetEffectiveValue();
	AxTexture2D *texture = (AxTexture2D*)this->properties[AxNormalMapTextureMechanism::propertyIndex_Texture]->GetEffectiveValue();

	if ((sourceHeightMap == 0) || (texture == 0))
		return;

	AxImage *sourceHeightMapImage = sourceHeightMap->ToImage();

	AxImage image;
	AxGenerateNormalMapImage(image, *sourceHeightMapImage, filter, emphasis);

	delete sourceHeightMapImage;
	
	texture->Load(image);
}