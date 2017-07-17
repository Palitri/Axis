//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxAssimpSceneDispatcher.h"

#include "AxAssimpScene.h"

AxAssimpSceneDispatcher::AxAssimpSceneDispatcher(void)
{
}


AxAssimpSceneDispatcher::~AxAssimpSceneDispatcher(void)
{
}

bool AxAssimpSceneDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
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
			info = "Assimp library formats";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Deserializes and serialized 3d file formats using the Assimp (Open Asset Import) Library. Import overs 30 formats.";
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

void *AxAssimpSceneDispatcher::CreateObject()
{
	return new AxAssimpScene();
}
