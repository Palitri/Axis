//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWindowsFormsInputDeviceDispatcher.h"

#include "AxWindowsFormsInputDevice.h"

AxWindowsFormsInputDeviceDispatcher::AxWindowsFormsInputDeviceDispatcher(void)
{
}


AxWindowsFormsInputDeviceDispatcher::~AxWindowsFormsInputDeviceDispatcher(void)
{
}

bool AxWindowsFormsInputDeviceDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_InputDevice);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Windows forms input";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Windows forms input device utilizes the keyboard and mouse input received from a Windows form";
			return true;
	}

	return false;
}

void *AxWindowsFormsInputDeviceDispatcher::CreateObject()
{
	return new AxWindowsFormsInputDevice();
}
