#pragma once
class ITask
{
public:
	virtual ~ITask();
	virtual void Execute() = 0;
	int exitCode = 0;
};

ITask::~ITask()
{
}