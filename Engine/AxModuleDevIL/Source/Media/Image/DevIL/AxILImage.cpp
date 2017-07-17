//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxILImage.h"

#include <DevIL\il.h>

AxILImage::Initialization AxILImage::initialization;

AxILImage::Initialization::Initialization(void)
{
	ilInit();

	// Set origin to lower left. Works only for loading images. Default is upper left. Saving seems to be lower left and unchangable
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
}

AxILImage::AxILImage(void)
	: AxImage()
{
	this->SetPixelFormat(AxPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat::ChannelIdRed, AxPixelFormat::ChannelIdGreen, AxPixelFormat::ChannelIdBlue, AxPixelFormat::ChannelIdAlpha));
}

AxILImage::~AxILImage(void)
{
}

bool AxILImage::Load(AxStream &source)
{
	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);
	
	void *bytes = AxMem::Get((int)source.length);
	source.ReadData(bytes, source.length);
	bool result = ilLoadL(IL_TYPE_UNKNOWN, bytes, (unsigned int) source.length) == IL_TRUE;
	AxMem::FreeAndNull(&bytes);

	if (result)
	{
		this->SetSize(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), ilGetInteger(IL_IMAGE_DEPTH));
	
		ilCopyPixels(0, 0, 0, this->width, this->height, this->depth, IL_RGBA, IL_UNSIGNED_BYTE, this->pixelData);
	}

	ilDeleteImages(1, &imageId);

	return result;
}

bool AxILImage::Save(AxStream &destination, const char *format)
{
	ILuint ilEncoding;

	if (format != 0)
	{
		AxString encodingCode = AxString(format).Trim('.').ToLower();
		
		if (encodingCode.length == 0)
			ilEncoding = IL_PNG;

		if (encodingCode.Equals("png"))
			ilEncoding = IL_PNG;
		else if (encodingCode.Equals("jpg"))
			ilEncoding = IL_JPG;
		else if (encodingCode.Equals("bmp"))
			ilEncoding = IL_BMP;
		else if (encodingCode.Equals("tga"))
			ilEncoding = IL_TGA;
		else if (encodingCode.Equals("dds"))
			ilEncoding = IL_DDS;
		else
			return false;
	}
	else
		ilEncoding = IL_PNG;

	ILuint numChannels = this->pixelFormat.numChannels == 4 ? 4 : 3;
	ILuint ilFormat = numChannels == 4 ? IL_RGBA : IL_RGB;
	AxPixelFormat requiredFormat(numChannels, numChannels, 8, 8, 8, 8, AxPixelFormat::ChannelIdRed, AxPixelFormat::ChannelIdGreen, AxPixelFormat::ChannelIdBlue, AxPixelFormat::ChannelIdAlpha);
	AxImage *image = this->pixelFormat.Equals(requiredFormat) ? this : new AxImage(*this, requiredFormat);

	ILuint imageId;
	ilGenImages(1, &imageId);
	ilBindImage(imageId);
	ilTexImage(image->width, image->height, image->depth, numChannels, ilFormat, IL_UNSIGNED_BYTE, image->pixelData);
	unsigned int dataSize = ilSaveL(ilEncoding, 0, 0);
	char *data = new char[dataSize];
	dataSize = ilSaveL(ilEncoding, data, dataSize);
	ilDeleteImages(1, &imageId);
	destination.WriteData(data, dataSize);
	delete[] data;

	if (image != this)
        delete image;

    return dataSize != 0;
}
