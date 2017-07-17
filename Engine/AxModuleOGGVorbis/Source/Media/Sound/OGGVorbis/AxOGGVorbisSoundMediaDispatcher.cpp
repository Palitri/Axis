//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOGGVorbisSoundMediaDispatcher.h"

#include "AxOGGVorbisSoundMedia.h"

AxOGGVorbisSoundMediaDispatcher::AxOGGVorbisSoundMediaDispatcher(void)
{
}


AxOGGVorbisSoundMediaDispatcher::~AxOGGVorbisSoundMediaDispatcher(void)
{
}


bool AxOGGVorbisSoundMediaDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Sound);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "OGG Vorbis";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "OGG Vorbis sound decoder";
			return true;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatExtension:
			if (infoTag == 0)
			{
				info = "ogg";
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatDescription:
			if (infoTag == 0)
			{
				info = "OGG Vorbis";
				return true;
			}
			break;
	}

	return false;
}

void *AxOGGVorbisSoundMediaDispatcher::CreateObject()
{
	return new AxOGGVorbisSoundMedia();
}
