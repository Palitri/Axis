//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxCheckerTextureMechanism.h"

#include "..\..\Textures\AxTexture2D.h"

#include "..\..\..\..\Media\Image\AxImage.h"

#include "..\..\..\Utilities\Imaging\AxCheckerPatternImage.h"

#include "..\..\..\..\Tools\AxMath.h"

AxCheckerTextureMechanism::AxCheckerTextureMechanism(void)
{
	this->typeId = AxCheckerTextureMechanism::classId;

	this->properties.Add(new AxProperty("Width", 256));
	this->properties.Add(new AxProperty("Height", 256));
	this->properties.Add(new AxProperty("Square size", 32));
	this->properties.Add(new AxProperty("Color 1", AxVector4(1.0f, 1.0f, 1.0f, 1.0f)));
	this->properties.Add(new AxProperty("Color 2", AxVector4(0.5f, 0.5f, 0.5f, 1.0f)));
    this->properties.Add(new AxProperty("Texture", 0, AxParameterType_ReferenceTexture));
}


AxCheckerTextureMechanism::~AxCheckerTextureMechanism(void)
{
}

void AxCheckerTextureMechanism::OnPropertiesChanged()
{
	int width = this->properties[AxCheckerTextureMechanism::propertyIndex_Width]->GetInt();
	int height = this->properties[AxCheckerTextureMechanism::propertyIndex_Height]->GetInt();
	int squareSize = this->properties[AxCheckerTextureMechanism::propertyIndex_SquareSize]->GetInt();
	AxVector4 color1 = this->properties[AxCheckerTextureMechanism::propertyIndex_Color1]->GetColor();
	AxVector4 color2 = this->properties[AxCheckerTextureMechanism::propertyIndex_Color2]->GetColor();
	AxTexture2D *texture = (AxTexture2D*)this->properties[AxCheckerTextureMechanism::propertyIndex_Texture]->GetEffectiveValue();

	if (texture == 0)
		return;

	AxImage image;
	AxCheckerPatternImage(image, width, height, squareSize, color1, color2);

	texture->Load(image);	
}