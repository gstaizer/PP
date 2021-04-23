#pragma once
#include "IWorker.h"
#include <windows.h>
#include <iostream>

class ThreadHandler : public IWorker
{
public:
	~ThreadHandler();
	bool ExecuteTask(ITask* task) override;
	bool IsBusy() override;
	void Terminate();
	void GetCreateThread(ITask* task);

private:
	bool isDelete = false;
	HANDLE handle = nullptr;
	static DWORD WINAPI StartThread(CONST LPVOID lpParam);
};

ThreadHandler::~ThreadHandler()
{
	isDelete = true;
	WaitForSingleObject(handle, INFINITE);
}

DWORD WINAPI ThreadHandler::StartThread(CONST LPVOID lpParam)
{
	ITask* task = (ITask*)lpParam;
	task->Execute();

	ExitThread(0); // функция устанавливает код завершения потока в 0
}

void ThreadHandler::Terminate()
{
	TerminateThread(handle, 1);
	currentTask->exitCode = 1;
}

void ThreadHandler::GetCreateThread(ITask* task)
{
	currentTask = task;
	handle = CreateThread(NULL, 0, &StartThread, task, 0, NULL);
}

bool ThreadHandler::ExecuteTask(ITask* task)
{
	if (isDelete)
	{
		return false;
	}

	if (IsBusy())
	{
		Terminate();
		GetCreateThread(task);
		return false;
	}
	else
	{
		GetCreateThread(task);
		return true;
	}
}

bool ThreadHandler::IsBusy()
{
	DWORD exitCode;

	GetExitCodeThread(handle, &exitCode);
	if (exitCode == STILL_ACTIVE)
	{
		return true;
	}
	return false;
}