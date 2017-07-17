//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMutex.h"

#include <windows.h>

AxMutex::AxMutex(void)
{
	this->handle = new HANDLE;

	*(HANDLE*)this->handle = CreateMutex( 
		NULL,		// default security attributes
		FALSE,		// initially not owned
		NULL);		// unnamed mutex
}


AxMutex::~AxMutex(void)
{
	CloseHandle(*(HANDLE*)this->handle);

	delete this->handle;
}

bool AxMutex::Lock()
{
	return WaitForSingleObject(*(HANDLE*)this->handle, INFINITE) == WAIT_OBJECT_0;
}

void AxMutex::Unlock()
{
	ReleaseMutex(*(HANDLE*)this->handle);
}