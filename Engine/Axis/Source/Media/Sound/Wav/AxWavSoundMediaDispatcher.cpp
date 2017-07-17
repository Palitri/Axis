//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWavSoundMediaDispatcher.h"

#include "AxWavSoundMedia.h"

AxWavSoundMediaDispatcher::AxWavSoundMediaDispatcher(void)
{
}


AxWavSoundMediaDispatcher::~AxWavSoundMediaDispatcher(void)
{
}

bool AxWavSoundMediaDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Sound);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Wave";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Axis built in reader of the Waveform Audio File Format";
			return true;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatExtension:
			if (infoTag == 0)
			{
				info = "wav";
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatDescription:
			if (infoTag == 0)
			{
				info = "Wave";
				return true;
			}
			break;
	}

	return false;
}

void *AxWavSoundMediaDispatcher::CreateObject()
{
	return new AxWavSoundMedia();
}
