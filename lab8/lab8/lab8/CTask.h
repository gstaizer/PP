#pragma once
#include "CBlur.h"

class CTask
{
public:
	CTask(CBlur inputImage);
	void ExecuteTask();
private:
	CBlur m_blurTask;
};