//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxILImageDispatcher.h"

#include "AxILImage.h"

AxILImageDispatcher::AxILImageDispatcher(void)
{
}


AxILImageDispatcher::~AxILImageDispatcher(void)
{
}

bool AxILImageDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	const int numDeserializationFormats = 7;
	const char *deserializationFormats[numDeserializationFormats] = { "bmp", "dds", "jpg", "jpeg", "jp2", "png", "tga" };
	const char *deserializationFormatsDescription[numDeserializationFormats] = { "Windows Bitmap", "DirectDraw Surface", "Jpeg", "Jpeg", "Jpeg 2000", "Portable Network Graphics", "Truevision Targa" };
	
	const int numSerializationFormats = 5;
	const char *serializationFormats[numSerializationFormats] = { "bmp", "dds", "jpg", "png", "tga" };
	const char *serializationFormatsDescription[numSerializationFormats] = { "Windows Bitmap", "DirectDraw Surface", "Jpeg", "Portable Network Graphics", "Truevision Targa" };
	
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Image);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "DevIL";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Serializes and deserializes images using DevIL - the open Developers Image Library (a.k.a. OpenIL)";
			return true;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatExtension:
			if ((infoTag >= 0) && (infoTag < numDeserializationFormats))
			{
				info = deserializationFormats[infoTag];
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatDescription:
			if ((infoTag >= 0) && (infoTag < numDeserializationFormats))
			{
				info = deserializationFormatsDescription[infoTag];
				return true;
			}
			break;
		
		case AxModuleDispatcher::infoId_MediaSerializationFormatExtension:
			if ((infoTag >= 0) && (infoTag < numSerializationFormats))
			{
				info = serializationFormats[infoTag];
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaSerializationFormatDescription:
			if ((infoTag >= 0) && (infoTag < numSerializationFormats))
			{
				info = serializationFormatsDescription[infoTag];
				return true;
			}
			break;	
	}

	return false;
}

void *AxILImageDispatcher::CreateObject()
{
	return new AxILImage();
}
