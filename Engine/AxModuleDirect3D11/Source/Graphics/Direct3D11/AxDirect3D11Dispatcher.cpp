//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDirect3D11Dispatcher.h"

#include "AxDirect3D11GraphicsDevice.h"

AxDirect3D11Dispatcher::AxDirect3D11Dispatcher(void)
{
}


AxDirect3D11Dispatcher::~AxDirect3D11Dispatcher(void)
{
}

bool AxDirect3D11Dispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_GraphicsDevice);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Direct3D 11";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Direct3D 11 rendering device with dynamic shader building. Provides a full-feature support for the Axis engine.";
			return true;
	}

	return false;
}

void *AxDirect3D11Dispatcher::CreateObject()
{
	return new AxDirect3D11GraphicsDevice();
}
