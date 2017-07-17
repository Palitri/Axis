//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxObjSceneDispatcher.h"

#include "AxObjScene.h"

AxObjSceneDispatcher::AxObjSceneDispatcher(void)
{
}


AxObjSceneDispatcher::~AxObjSceneDispatcher(void)
{
}

bool AxObjSceneDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	const int numDeserializationFormats = 2;
	const char *deserializationFormats[numDeserializationFormats] = { "obj", "mtl" };
	const char *deserializationFormatsDescription[numDeserializationFormats] = { "Wavefront mesh", "Wavefront material" };
	
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Scene);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Wavefront OBJ and MTL";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Axis built in reader of Wavefront's OBJ and MTL formats";
			return true;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatExtension:
			if ((infoTag >= 0) && (infoTag < numDeserializationFormats))
			{
				info = deserializationFormats[infoTag];
				return true;
			}
			break;

		case AxModuleDispatcher::infoId_MediaDeserializationFormatDescription:
			if ((infoTag >= 0) && (infoTag < numDeserializationFormats))
			{
				info = deserializationFormatsDescription[infoTag];
				return true;
			}
			break;
	}

	return false;
}

void *AxObjSceneDispatcher::CreateObject()
{
	return new AxObjScene();
}
