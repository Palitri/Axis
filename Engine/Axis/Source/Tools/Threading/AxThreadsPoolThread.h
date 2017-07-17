#pragma once

#include "..\..\AxGlobals.h"

#include "AxThread.h"
#include "AxMutex.h"

class AXDLLCLASS ThreadsPoolTask
{
public:
	ThreadExecuteFunction function;
	void *functionParameter;
	int id;

	ThreadsPoolTask(void);
	ThreadsPoolTask(ThreadExecuteFunction function, void *functionParameter, int id);
};

class AxThreadsPool;

class AXDLLCLASS AxThreadsPoolThread :
	public AxThread
{
public:
	AxThreadsPool *pool;
	ThreadsPoolTask task;

	AxThreadsPoolThread(AxThreadsPool *pool);
	virtual ~AxThreadsPoolThread(void);

	virtual int Execute();
};

