#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <Windows.h>
#include <vector>
#include <ctime>
#include "../../easyBMP/EasyBMP.h"
#include "CLogBuffer.h"

using namespace std;

struct ThreadInfo
{
	ThreadInfo(const ThreadInfo& thread) {};
	ThreadInfo() {};
	~ThreadInfo() {};
	BMP inputImage;
	BMP* outputImage;
	vector<vector<RGBApixel>> pixels;
	int index;
	int threadCount;
	clock_t startTime;
};

class CBlur
{
public:
	CBlur(const char* inputImage, const char* outputImage, int threadCount, int coreCount);

	void StartBluring(clock_t start);
private:
	const char* m_inputImage;
	const char* m_outputImage;
	int m_threadCount, m_coreCount;
	vector<int> m_threadPriorities = {};
	bool isPrioritiesReaded();
};