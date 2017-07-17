#pragma once

#include "..\Tools\AxString.h"

struct AxSerializationParameters
{
	// The root dir for the serialization. Used as a base for relative paths. Can be used when loading from a stream, when fullSourceName is not available
	AxString rootDir;
	// A friendly name of the serialization source. Used when such is needed, for example to give names to resources
	AxString friendlyName;
	// The full name of the source. Generally, the path with file name.
	AxString fullSourceName;
};