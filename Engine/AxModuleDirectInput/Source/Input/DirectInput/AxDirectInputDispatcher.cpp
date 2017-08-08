//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDirectInputDispatcher.h"

#include "AxDirectInputDevice.h"

AxDirectInputDispatcher::AxDirectInputDispatcher(void)
{
}


AxDirectInputDispatcher::~AxDirectInputDispatcher(void)
{
}

bool AxDirectInputDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_InputDevice);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "DirectInput";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "DirectInput device with support for generic joystick and gamepad";
			return true;
	}

	return false;
}

void *AxDirectInputDispatcher::CreateObject()
{
	return new AxDirectInputDevice();
}
