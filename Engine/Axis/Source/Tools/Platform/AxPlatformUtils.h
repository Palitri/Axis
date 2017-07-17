#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxString.h"
#include "..\AxStrings.h"

class AXDLLCLASS AxPlatformUtils
{
public:
	AxPlatformUtils(void);
	~AxPlatformUtils(void);

    static long long GetPerformanceCounter();
    static long long GetPerformanceCounterFrequency();

    static void ShowMessage(const wchar_t *text, const wchar_t *caption = 0, void *windowHandle = 0);

	static void GetScreenSize(int &width, int &height);
	static int GetNumberOfDisplays();
	
	static bool GetWindowClientSize(void *windowHandle, int &width, int &height);

	static int GetNumberOfProcessors();

	static void SleepX(int milliseconds);

	static AxString GetCurrentDir();

	static AxString GetApplicationName();
	static AxString GetApplicationPath();

	static void *LoadModule(AxString &fileName);
	static void UnloadModule(void **module);
	static void *GetModuleFunction(void *module, AxString &functionName);
};

