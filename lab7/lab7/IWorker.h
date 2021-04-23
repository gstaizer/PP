#pragma once
#include "ITask.h"
class IWorker
{
public:
	virtual ~IWorker();
	virtual bool ExecuteTask(ITask* task) = 0;
	virtual bool IsBusy() = 0;

protected:
	ITask* currentTask = nullptr;
};

IWorker::~IWorker()
{
}