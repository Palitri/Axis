#pragma warning(disable:4231)
#pragma once

#if defined (AXDLLCOMPILE)
#	define AXDLLCLASS __declspec(dllexport)
#	define AXDLLTEMPLATEMEMBER
#else
#	define AXDLLCLASS __declspec(dllimport)
#	define AXDLLTEMPLATEMEMBER extern
#endif
