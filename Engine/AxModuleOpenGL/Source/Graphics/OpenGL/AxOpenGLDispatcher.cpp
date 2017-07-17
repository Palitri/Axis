//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGLDispatcher.h"

#include "AxOpenGLGraphicsDevice.h"

AxOpenGLDispatcher::AxOpenGLDispatcher(void)
{
}


AxOpenGLDispatcher::~AxOpenGLDispatcher(void)
{
}


bool AxOpenGLDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_GraphicsDevice);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "OpenGL";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "OpenGL rendering device";
			return true;
	}

	return false;
}

void *AxOpenGLDispatcher::CreateObject()
{
	return new AxOpenGLGraphicsDevice();
}
