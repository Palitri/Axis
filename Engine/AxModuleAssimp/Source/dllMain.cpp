//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "Media\Scene\Assimp\AxAssimpSceneDispatcher.h"

extern "C"
{

	__declspec(dllexport) unsigned int AxGetSignature()
	{
		return Axis::VersionSignature;
	}

	__declspec(dllexport) void *AxCreateDispatcher(int index)
	{
		switch (index)
		{
			case 0:
			{
				return new AxAssimpSceneDispatcher();
			}
		}

		return 0;
	}

}
