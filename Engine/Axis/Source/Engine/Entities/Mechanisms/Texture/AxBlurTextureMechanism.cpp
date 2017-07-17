//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxBlurTextureMechanism.h"

#include "..\..\Textures\AxTexture2D.h"

#include "..\..\..\..\Media\Image\AxImage.h"

#include "..\..\..\Utilities\Imaging\AxGaussianBlurImage.h"

#include "..\..\..\..\Tools\AxMath.h"

AxBlurTextureMechanism::AxBlurTextureMechanism(void)
{
	this->typeId = AxBlurTextureMechanism::classId;

	this->properties.Add(new AxProperty("Blurriness", 5.0f));
    this->properties.Add(new AxProperty("Original texture", 0, AxParameterType_ReferenceTexture));
    this->properties.Add(new AxProperty("Texture", 0, AxParameterType_ReferenceTexture));
}


AxBlurTextureMechanism::~AxBlurTextureMechanism(void)
{
}


void AxBlurTextureMechanism::OnPropertiesChanged()
{
	float blurriness = this->properties[AxBlurTextureMechanism::propertyIndex_Blurriness]->GetFloat();
	AxTexture2D *originalTexture = (AxTexture2D*)this->properties[AxBlurTextureMechanism::propertyIndex_OriginalTexture]->GetEffectiveValue();
	AxTexture2D *texture = (AxTexture2D*)this->properties[AxBlurTextureMechanism::propertyIndex_Texture]->GetEffectiveValue();

	if ((originalTexture == 0) || (texture == 0))
		return;

	AxImage *originalImage = originalTexture->ToImage();

	AxImage image;
	// Kernel size should be an odd number, so that it has a middle element, and ideally about 6 times the size of the standardDeviation (as elements in the kernel which are further than 3 times the standardDeviation from the center are close to zero)
	int kernelSize = AxMath::Max(((int)(blurriness * 3.0f * 2.0f) * 2) / 2 + 1, 1);
	AxGaussianBlurImage(image, *originalImage, kernelSize, blurriness);

	delete originalImage;

	texture->Load(image);
}
