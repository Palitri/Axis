#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxList.h"

#include "AxThreadsPoolThread.h"
#include "AxMutex.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxThreadsPoolThread*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<ThreadsPoolTask>;

class AXDLLCLASS AxThreadsPool
{
	friend class AxThreadsPoolThread;

private:
	int nextTaskId;

	// If there is a suspnded thread, then resume it. The resumed thread will pull a task form the queue. If there is no suspended thread, this means that all tasks are working and as soon as any of them finishes, it will pull a task from the queue
	int ResumeSuspendedThread();
	// Pulls a task from the queue. If there are tasks in the queue, the pulled task is returned in the "task" parameter and the method returns true. If there are no tasks in the queue, the method returns false.
	bool PullQueuedTask(ThreadsPoolTask &task);

public:
	AxList<AxThreadsPoolThread*> threads;//, freeThreads;
	AxList<ThreadsPoolTask> queuedTasks;
	AxMutex synchronization;

	AxThreadsPool(int numThreads);
	~AxThreadsPool(void);

	// Queues a task to be executed and returns the task's id. It will be executed as soon as there is a free thread.
	int QueueTask(ThreadExecuteFunction function, void *functionParameter);

	// Checks if the task with the given id is currently being executed
	bool Executing(int taskId);
	// Checks if the task with the given id is still pending on the queue, waiting for for a thread in the pool to get free and execute it
	bool Pending(int taskId);
	// Checks if the task with the given id has been fully executed
	bool Finished(int taskId);
	// Checks if the pool is entirely free, with all its threads idle
	bool Idle();
	
	// Waits until the task with the given id is executed. Returns after the task has been completed.
	void Wait(int taskId);
	// Waits until all the queued tasks are fully completed.
	void WaitAll();
};

