//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNativeJSSceneDispatcher.h"

#include "AxNativeJSScene.h"


AxNativeJSSceneDispatcher::AxNativeJSSceneDispatcher(void)
{
}


AxNativeJSSceneDispatcher::~AxNativeJSSceneDispatcher(void)
{
}

bool AxNativeJSSceneDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Scene);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Axis JS";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Axis native scene format for JavaScript embedding. Serializes and deserializes scene in Axis' native format, encoded as a Base64 string. This format can be easily embedded inside an HTML page or loaded from the .js file itself. Useful for pages meant to be contained in a single HTML file or to be opened locally without a web server";
			return true;

		case AxModuleDispatcher::infoId_MediaSerializationFormatExtension:
			if (infoTag == 0)
			{
				info = AxNativeJSScene::formatName;
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaSerializationFormatDescription:
			if (infoTag == 0)
			{
				info = "Axis JS";
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatExtension:
			if (infoTag == 0)
			{
				info = AxNativeJSScene::formatName;
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatDescription:
			if (infoTag == 0)
			{
				info = "Axis JS";
				return true;
			}
			break;
	}

	return false;
}

void *AxNativeJSSceneDispatcher::CreateObject()
{
	return new AxNativeJSScene();
}
