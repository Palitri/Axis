//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "Ax3dsSceneDispatcher.h"

#include "Ax3dsScene.h"

Ax3dsSceneDispatcher::Ax3dsSceneDispatcher(void)
{
}


Ax3dsSceneDispatcher::~Ax3dsSceneDispatcher(void)
{
}

bool Ax3dsSceneDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Scene);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "3D Studio Max";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Axis built in reader of the 3D Studio Max format";
			return true;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatExtension:
			if (infoTag == 0)
			{
				info = "3ds";
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatDescription:
			if (infoTag == 0)
			{
				info = "3D Studio Max";
				return true;
			}
			break;
	}

	return false;
}

void *Ax3dsSceneDispatcher::CreateObject()
{
	return new Ax3dsScene();
}
