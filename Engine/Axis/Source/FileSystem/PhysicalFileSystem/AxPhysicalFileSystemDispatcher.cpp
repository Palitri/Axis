//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPhysicalFileSystemDispatcher.h"

#include "AxPhysicalFileSystem.h"


AxPhysicalFileSystemDispatcher::AxPhysicalFileSystemDispatcher(void)
{
}


AxPhysicalFileSystemDispatcher::~AxPhysicalFileSystemDispatcher(void)
{
}

bool AxPhysicalFileSystemDispatcher::GetInfo(unsigned int infoId, int infoTag, AxString &info)
{
	switch (infoId)
	{
		case AxModuleDispatcher::infoId_TypeId:
			info = AxString(AxModuleDispatcher::typeId_FileSystem);
			return true;

		case AxModuleDispatcher::infoId_Name:
			info = "Physical file system";
			return true;

		case AxModuleDispatcher::infoId_Description:
			info = "Physical file system uses the local file system of the host machine";
			return true;
	}

	return false;
}

void *AxPhysicalFileSystemDispatcher::CreateObject()
{
	return new AxPhysicalFileSystem();
}
