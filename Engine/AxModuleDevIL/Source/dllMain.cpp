//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------


// Important:
// The Debug Configuration Properties' VC++ Directories setting has the typical for other modules $(OutDir) replaced by $(SolutionDir)\Release. This is so that in Debug mode, the projects looks for the release version of Axis.lib instead of the debug version.
// Also, the Visual Leak Detector is not used for this project.
// The reason behind this is that DevIL library 1.7.8 seems to have a problem with VLD 2.5 and using both in one application causes it to crash immediately when started (probably when linking with the .dlls, before even running any of the application's code)
// An example crash message: Unhandled exception at 0x032374B3 (vld_x86.dll) in DevILTest.exe: 0xC0000005: Access violation reading location 0x4E52454B
// Earlier version of DevIL (like 1.6.7) doesn't have this problem with VLD 2.5, but it doesn't have the ilSaveL functionality of pre-determining the size of a saved image, needed when allocating memory for saving images.
// Earlier version of VLD (like 2.2.3) doesn't have this problem with DevIL 1.7.8, but still, using VLD 2.2.3 with DevIL 1.7.8 causes some very strange undefined problems with the editor in QT. Although debugging a small test program in Visual Studio doesn't exhibit problems, this still probably shows a sign that there actually is a hidden problem.
// Also, when a module project, like AxModuleDevIL refers to Axis by using the debug version of Axis.lib, it effectively uses VLD (because Axis uses it) and even if VLD is not explicitly used by AxModuleDevIL, it actually is - indirectly through Axis
// To get around these problems, this particular module uses the Release version of Axis.lib even for its debug configuration, thus the setting for VC++ Directories and doesn't use VLD explicitly

#include "vld\vld.h"

#include "Media\Image\DevIL\AxILImageDispatcher.h"

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
				return new AxILImageDispatcher();
			}
		}

		return 0;
	}

}