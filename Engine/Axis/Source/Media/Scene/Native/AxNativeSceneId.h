#pragma once

#include "..\..\..\AxGlobals.h"

class AXDLLCLASS AxNativeSceneID
{
public:
    static const unsigned int Main						= 0x53495841; // AXIS

	static const unsigned int Header					= 0x10000000;
	static const unsigned int Header_String				= 0x11000000;
	static const unsigned int Header_Ver				= 0x12000000;

	static const unsigned int Resources					= 0x20000000;
	static const unsigned int ResourceSerializationWrap	= 0x21000000;

	static const unsigned int RootBranch				= 0x30000000;

	static const unsigned int RuntimeParameters			= 0x40000000;
	static const unsigned int Settings					= 0x50000000;
	static const unsigned int Thumbnail					= 0x60000000;
};
