//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFbxSceneDispatcher.h"

#include "AxFbxScene.h"

AxFbxSceneDispatcher::AxFbxSceneDispatcher(void)
{
}


AxFbxSceneDispatcher::~AxFbxSceneDispatcher(void)
{
}

bool AxFbxSceneDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	const int numDeserializationFormats = 4;
	const char *deserializationFormats[numDeserializationFormats] = { "fbx", "dae", "3ds", "obj" };
	const char *deserializationFormatsDescription[numDeserializationFormats] = { "Autodesk FBX", "Collada DAE", "3D Studio", "Wavefront OBJ" };
	
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_Scene);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Autodesk FBX formats";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Deserializes 3d file formats using the Autodesk FBX SDK. Supports FBX, DAE, 3DS and OBJ. Can only load data from files from the physical file system.";
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

void *AxFbxSceneDispatcher::CreateObject()
{
	return new AxFbxScene();
}
