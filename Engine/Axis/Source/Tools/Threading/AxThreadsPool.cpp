//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxThreadsPool.h"

#include "..\Platform\AxPlatformUtils.h"

#include <windows.h>

AxThreadsPool::AxThreadsPool(int numThreads)
{
	for (int i = 0; i < numThreads; i++)
	{
		AxThreadsPoolThread *thread = new AxThreadsPoolThread(this);

		this->threads.Add(thread);
		//this->freeThreads.Add(thread);
	}

	this->nextTaskId = 0;

}


AxThreadsPool::~AxThreadsPool(void)
{
	for (int i = 0; i < this->threads.count; i++)
	{
		this->threads[i]->Finish();
	}

	for (int i = 0; i < this->threads.count; i++)
	{
		delete this->threads[i];
	}
}

int AxThreadsPool::ResumeSuspendedThread()
{
	this->synchronization.Lock();

	for (int i = 0; i < this->threads.count; i++)
	{
		if (!this->threads[i]->running)
		{
			this->threads[i]->Run();
			this->synchronization.Unlock();
			return i;
		}
	}

	this->synchronization.Unlock();

	return -1;
}

bool AxThreadsPool::PullQueuedTask(ThreadsPoolTask &task)
{
	this->synchronization.Lock();

	bool result = this->queuedTasks.count != 0;

	if (result)
	{
		task = this->queuedTasks[0];
		this->queuedTasks.RemoveAt(0);
	}

	this->synchronization.Unlock();

	return result;
}

int AxThreadsPool::QueueTask(ThreadExecuteFunction function, void *functionParameter)
{
	this->synchronization.Lock();

	int taskId = this->nextTaskId++;

	this->queuedTasks.Add(ThreadsPoolTask(function, functionParameter, taskId));

	this->ResumeSuspendedThread();

	this->synchronization.Unlock();

	return taskId;
}

bool AxThreadsPool::Pending(int taskId)
{
	this->synchronization.Lock();

	bool result = false;

	for (int i = 0; i < this->queuedTasks.count; i++)
	{
		if (this->queuedTasks[i].id == taskId)
		{
			result = true;
			break;
		}
	}

	this->synchronization.Unlock();

	return result;
}

bool AxThreadsPool::Executing(int taskId)
{
	this->synchronization.Lock();

	bool result = false;

	for (int i = 0; i < this->threads.count; i++)
	{
		if ((this->threads[i]->task.id == taskId) && this->threads[i]->running)
		{
			result = true;
			break;
		}
	}

	this->synchronization.Unlock();

	return result;
}

bool AxThreadsPool::Finished(int taskId)
{
	return !this->Executing(taskId) && !this->Pending(taskId);
}


bool AxThreadsPool::Idle()
{
	if (this->queuedTasks.count != 0)
		return false;

	bool result = true;

	this->synchronization.Lock();

	for (int i = 0; i < this->threads.count; i++)
	{
		if (this->threads[i]->running)
		{
			result = false;
			break;
		}
	}

	this->synchronization.Unlock();

	return result;
}

void AxThreadsPool::Wait(int taskId)
{
	while (!this->Finished(taskId))
	{
		AxPlatformUtils::SleepX(0);
	}
}

void AxThreadsPool::WaitAll()
{
	while (!this->Idle())
	{
		AxPlatformUtils::SleepX(0);
	}
}
