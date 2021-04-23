#pragma once
#include "ITask.h"
#include <iostream>
#include <string>

using namespace std;

class GetTask : public ITask
{
public:
	GetTask(string str);
	void Execute() override;

private:
	string text = "";
};

GetTask::GetTask(string str)
{
	text = str;
}

void GetTask::Execute()
{
	cout << text;
}