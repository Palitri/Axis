//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNativeSceneDispatcher.h"

#include "AxNativeScene.h"

AxNativeSceneDispatcher::AxNativeSceneDispatcher(void)
{
}


AxNativeSceneDispatcher::~AxNativeSceneDispatcher(void)
{
}

bool AxNativeSceneDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Scene);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Axis";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Axis native scene format. Serializes and deserializes scene in Axis' native format";
			return true;

		case AxModuleDispatcher::infoId_MediaSerializationFormatExtension:
			if (infoTag == 0)
			{
				info = AxNativeScene::formatName;
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaSerializationFormatDescription:
			if (infoTag == 0)
			{
				info = "Axis";
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatExtension:
			if (infoTag == 0)
			{
				info = AxNativeScene::formatName;
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatDescription:
			if (infoTag == 0)
			{
				info = "Axis";
				return true;
			}
			break;
	}

	return false;
}

void *AxNativeSceneDispatcher::CreateObject()
{
	return new AxNativeScene();
}
