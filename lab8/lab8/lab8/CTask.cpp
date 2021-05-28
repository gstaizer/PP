#include "CTask.h"

CTask::CTask(CBlur inputImage) : m_blurTask(inputImage) {}

void CTask::ExecuteTask()
{
	m_blurTask.StarBluring();
}