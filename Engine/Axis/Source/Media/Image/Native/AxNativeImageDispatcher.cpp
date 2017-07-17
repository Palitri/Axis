//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNativeImageDispatcher.h"

#include "AxNativeImage.h"

AxNativeImageDispatcher::AxNativeImageDispatcher(void)
{
}


AxNativeImageDispatcher::~AxNativeImageDispatcher(void)
{
}

bool AxNativeImageDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Image);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Axis image";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Axis native image format. Serializes and deserializes images in Axis' native format";
			return true;

		case AxModuleDispatcher::infoId_MediaSerializationFormatExtension:
			if (infoTag == 0)
			{
				info = AxNativeImage::formatName;
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaSerializationFormatDescription:
			if (infoTag == 0)
			{
				info = "Axis image";
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatExtension:
			if (infoTag == 0)
			{
				info = AxNativeImage::formatName;
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatDescription:
			if (infoTag == 0)
			{
				info = "Axis image";
				return true;
			}
			break;
	}

	return false;
}

void *AxNativeImageDispatcher::CreateObject()
{
	return new AxNativeImage();
}
