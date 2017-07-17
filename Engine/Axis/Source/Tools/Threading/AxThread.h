#pragma once

#include "..\..\AxGlobals.h"


typedef int (*ThreadExecuteFunction) (void *parameter);

class AXDLLCLASS AxThread
{
private:
	void *handle, *id;

	int Initialize(bool start, bool reusable);

public:
	void *signalEvent;

	bool reusable,		// Indicates that the thread is reusable and can be re-run multiple times
		 running,		// Indicates that the thread is running its task
		 terminated;	// Indicates that the thread has finished doing its work

	ThreadExecuteFunction function;
	void *finctionParameter;
		
	AxThread(bool start = true, bool reusable = false);
	AxThread(ThreadExecuteFunction function, void *functionParameter, bool start = true, bool reusable = false);
	virtual ~AxThread(void);

	// Runs the thread's task, calling the Execute() method
	void Run();
	// Signals the thread to terminate after finishing its task
	void Finish();
	// Waits until the thread has finished its task;
	void WaitTask();
	// Waits until the thread has terminated;
	void Wait();

	virtual int Execute();
};