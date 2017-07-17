//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGL1_1Dispatcher.h"

#include "AxOpenGL1_1GraphicsDevice.h"

AxOpenGL1_1Dispatcher::AxOpenGL1_1Dispatcher(void)
{
}


AxOpenGL1_1Dispatcher::~AxOpenGL1_1Dispatcher(void)
{
}


bool AxOpenGL1_1Dispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_GraphicsDevice);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "OpenGL 1.1";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "OpenGL 1.1 rendering device with fixed pipeline. Supports only the very basic rendering functionalities.";
			return true;
	}

	return false;
}

void *AxOpenGL1_1Dispatcher::CreateObject()
{
	return new AxOpenGL1_1GraphicsDevice();
}
