//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPlatformUtils.h"

#include "..\..\FileSystem\AxFileSystem.h"

#include <windows.h>
#include <direct.h>

#ifndef UNICODE
#define UNICODE
#endif

AxPlatformUtils::AxPlatformUtils(void)
{
}


AxPlatformUtils::~AxPlatformUtils(void)
{
}


long long AxPlatformUtils::GetPerformanceCounter()
{
	LARGE_INTEGER result;

	if (QueryPerformanceCounter(&result))
		return result.QuadPart;
	else
		return -1;
}

long long AxPlatformUtils::GetPerformanceCounterFrequency()
{
	LARGE_INTEGER result;

	if (QueryPerformanceFrequency(&result))
		return result.QuadPart;
	else
		return -1;
}

void AxPlatformUtils::ShowMessage(const wchar_t *text, const wchar_t *caption, void *windowHandle)
{
	HWND handle = windowHandle == 0 ? NULL : *(HWND*)windowHandle;

	MessageBoxW(handle, text, caption == 0 ? L"" : caption, MB_OK);
}

void AxPlatformUtils::GetScreenSize(int &width, int &height)
{
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
}

int AxPlatformUtils::GetNumberOfDisplays()
{
	return GetSystemMetrics(SM_CMONITORS);
}

bool AxPlatformUtils::GetWindowClientSize(void *windowHandle, int &width, int &height)
{
	HWND handle = windowHandle == 0 ? NULL : *(HWND*)windowHandle;

	RECT rect;
	bool result = GetClientRect(handle, &rect) != 0;

	width = rect.right;
	height = rect.bottom;

	return result;
}

int AxPlatformUtils::GetNumberOfProcessors()
{
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);

	return sysinfo.dwNumberOfProcessors;
}

void AxPlatformUtils::SleepX(int milliseconds)
{
	Sleep(milliseconds);
}

AxString AxPlatformUtils::GetCurrentDir()
{
	TCHAR buffer[MAX_PATH]={0};
	int length = GetCurrentDirectory(MAX_PATH, buffer);
	return AxString(AxString(buffer, AxString::GetLength(buffer)).Replace('\\', AxFileSystem::dirSplitter));
}

AxString AxPlatformUtils::GetApplicationName()
{
	TCHAR buffer[MAX_PATH]={0};
	GetModuleFileName(NULL, buffer, MAX_PATH);
	return AxString(AxString(buffer, AxString::GetLength(buffer)).Replace('\\', AxFileSystem::dirSplitter));
}

AxString AxPlatformUtils::GetApplicationPath()
{
	AxString applicationName = AxPlatformUtils::GetApplicationName();
	return AxString(applicationName.SubString(0, applicationName.LastIndexOf(AxFileSystem::dirSplitter)));
}

void *AxPlatformUtils::LoadModule(AxString &fileName)
{
	HINSTANCE dllHandle;
	dllHandle = LoadLibraryW(fileName.Replace(AxFileSystem::dirSplitter, '\\').GetContents());
	//dllHandle = LoadLibraryExW(fileName.GetContents(), 0, 0);
	int e = GetLastError();
	if (dllHandle == NULL)
		return 0;

	HINSTANCE *result = new HINSTANCE();
	*result = dllHandle;

	return result;
}

void AxPlatformUtils::UnloadModule(void **module)
{
	if (*module == 0)
		return;

	HINSTANCE *dllHandle = (HINSTANCE*)*module;

	if (*dllHandle == NULL)
		return;

	FreeLibrary(*dllHandle);  
	*dllHandle = NULL;

	delete *module;
	*module = 0;
}

void *AxPlatformUtils::GetModuleFunction(void *module, AxString &functionName)
{
	if (module == 0)
		return 0;

	HINSTANCE *dllHandle = (HINSTANCE*)module;

	if (*dllHandle == NULL)
		return 0;
	
	return GetProcAddress(*dllHandle, functionName.GetCharContents());
}

void *AxPlatformUtils::OpenComPort(AxString comPortName, int baudRate)
{
	HANDLE serialHandle = CreateFile(AxString(AxString("\\\\.\\") + comPortName).GetCharContents(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	// Do some basic settings
	DCB serialParams = { 0 };
	serialParams.DCBlength = sizeof(serialParams);

	GetCommState(serialHandle, &serialParams);
	serialParams.BaudRate = baudRate;//BAUD_9600;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;
	SetCommState(serialHandle, &serialParams);

	// Set timeouts
	COMMTIMEOUTS timeout = { 0 };
	timeout.ReadIntervalTimeout = 20;
	timeout.ReadTotalTimeoutMultiplier = 0;
	timeout.ReadTotalTimeoutConstant = 20;
	timeout.WriteTotalTimeoutMultiplier = 0;
	timeout.WriteTotalTimeoutConstant = 50;

	SetCommTimeouts(serialHandle, &timeout);

	HANDLE *result = new HANDLE();
	*result = serialHandle;

	return result;
}

void AxPlatformUtils::CloseComPort(void **comPortHandle)
{
	if (*comPortHandle == 0)
		return;

	HANDLE *serialHandle = (HANDLE*)*comPortHandle;

	if (*serialHandle == NULL)
		return;

	CloseHandle(*serialHandle);
	*serialHandle = NULL;

	delete *comPortHandle;
	*comPortHandle = 0;
};

int AxPlatformUtils::ReadComPort(void *comPortHandle, void *buffer, int bytesToRead)
{
	if (comPortHandle == 0)
		return 0;

	HANDLE *serialHandle = (HANDLE*)comPortHandle;

	DWORD result;
	if (FAILED(ReadFile(*serialHandle, buffer, bytesToRead, &result, NULL)))
		return 0;

	return result;
};

int AxPlatformUtils::WriteComPort(void *comPortHandle, void *buffer, int bytesToWrite)
{
	if (comPortHandle == 0)
		return 0;

	HANDLE *serialHandle = (HANDLE*)comPortHandle;

	DWORD result;
	if (FAILED(WriteFile(*serialHandle, buffer, bytesToWrite, &result, NULL)))
		return 0;

	return result;
};

