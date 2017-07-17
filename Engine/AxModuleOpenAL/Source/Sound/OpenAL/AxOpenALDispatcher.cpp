//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenALDispatcher.h"

#include "AxOpenALAudioDevice.h"

AxOpenALDispatcher::AxOpenALDispatcher(void)
{
}


AxOpenALDispatcher::~AxOpenALDispatcher(void)
{
}


bool AxOpenALDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_AudioDevice);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "OpenAL";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "OpenAL audio device";
			return true;
	}

	return false;
}

void *AxOpenALDispatcher::CreateObject()
{
	return new AxOpenALAudioDevice();
}
