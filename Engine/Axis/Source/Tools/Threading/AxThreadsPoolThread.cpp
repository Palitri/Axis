//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxThreadsPoolThread.h"

#include "AxThreadsPool.h"


ThreadsPoolTask::ThreadsPoolTask(void)
{
}

ThreadsPoolTask::ThreadsPoolTask(ThreadExecuteFunction function, void *functionParameter, int id)
{
	this->function = function;
	this->functionParameter = functionParameter;
	this->id = id;
}

AxThreadsPoolThread::AxThreadsPoolThread(AxThreadsPool *pool)
	: AxThread(false, true)
{
	this->pool = pool;
}


AxThreadsPoolThread::~AxThreadsPoolThread(void)
{
}

int AxThreadsPoolThread::Execute()
{
	while (this->pool->PullQueuedTask(this->task))
	{
		this->running = true;
		this->task.function(this->task.functionParameter);
	}

	return 0;
}