//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSorasGraphicsDeviceDispatcher.h"

#include "AxSorasGraphicsDevice.h"

AxSorasGraphicsDeviceDispatcher::AxSorasGraphicsDeviceDispatcher(void)
{
}


AxSorasGraphicsDeviceDispatcher::~AxSorasGraphicsDeviceDispatcher(void)
{
}


bool AxSorasGraphicsDeviceDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_GraphicsDevice);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Axis Soras";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Axis Soras is a software rasterization device with some customizable shading capabilities. Supports a few of Axis' less complex features to some extent.";
			return true;
	}

	return false;
}

void *AxSorasGraphicsDeviceDispatcher::CreateObject()
{
	return new AxSorasGraphicsDevice();
}
