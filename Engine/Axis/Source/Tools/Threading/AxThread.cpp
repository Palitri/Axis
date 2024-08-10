//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxThread.h"

#include <windows.h>

DWORD WINAPI ExecuteThread(LPVOID lpParam)
{
	AxThread *instance = (AxThread*)lpParam;

	int result = 0;
	
	while (!instance->terminated)
	{
		if (WaitForSingleObject(*(HANDLE*)instance->signalEvent, INFINITE) == WAIT_OBJECT_0)
		{
			if (instance->running)
			{
				// Run has been requested
				result = instance->Execute();

				ResetEvent(*(HANDLE*)instance->signalEvent);

				instance->running = false;

				instance->terminated = !instance->reusable;
			}
			else
			{
				// Finish has been requested
				instance->terminated = true;
				result = 0;
			}
		}
		else
		{
			// Timeout or other erroneous case
			instance->terminated = true;
			result = 1;
		}
	}
	
	return result;
}

int AxThread::Initialize(bool start, bool reusable)
{
	this->signalEvent = new HANDLE();
	*(HANDLE*)this->signalEvent = CreateEvent(NULL, TRUE, start ? TRUE : FALSE, NULL);

	this->handle = new HANDLE;
	this->id = new DWORD;
		
	this->reusable = reusable;
	this->running = start;
	this->terminated = false;

	*(HANDLE*)this->handle = CreateThread(
		NULL,										// default security attributes
		0,											// default stack size
		(LPTHREAD_START_ROUTINE) ExecuteThread,		// funciton
		this,										// parameter
		NULL,										// default creation flags
		(DWORD*)this->id);							// receive thread identifier

    if(*(HANDLE*)this->handle == NULL)
		return GetLastError();
	else
		return 0;
}

AxThread::AxThread(bool start, bool reusable)
{
	this->Initialize(start, reusable);

	this->function = 0;
}

AxThread::AxThread(ThreadExecuteFunction function, void *functionParameter, bool start, bool reusable)
{
	this->function = function;
	this->finctionParameter = functionParameter;

	this->Initialize(start, reusable);
}

AxThread::~AxThread(void)
{
	this->Wait();
    
	CloseHandle(*(HANDLE*)this->handle);

	CloseHandle(*(HANDLE*)this->signalEvent);
	delete this->signalEvent;

	delete this->handle;
	delete this->id;
}

void AxThread::Run()
{
	this->running = true;
	SetEvent(*(HANDLE*)this->signalEvent);
}

void AxThread::Finish()
{
	this->running = false;
	SetEvent(*(HANDLE*)this->signalEvent);
}

void AxThread::WaitTask()
{
	while (this->running)
		Sleep(0);
}

void AxThread::Wait()
{
	while (!this->terminated)
		Sleep(0);
}

//void AxThread::Suspend()
//{
//	if (this->suspended)
//		return;
//
//	this->suspended = true;
//
//	SuspendThread(*(HANDLE*)this->handle);
//}
//
//void AxThread::Resume()
//{
//	if (!this->suspended)
//		return;
//
//	this->suspended = false;
//
//	ResumeThread(*(HANDLE*)this->handle);
//}

void AxThread::Terminate()
{
	if (this->terminated)
		return;

	TerminateThread(*(HANDLE*)this->handle, -1);
	this->terminated = true;
}

int AxThread::Execute()
{
	if (this->function == 0)
		return 0;
	
	return this->function(this->finctionParameter);
}
