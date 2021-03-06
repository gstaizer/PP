#include "CBlur.h"
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void BlurProc(ThreadInfo* thread, int startBlur, int endBlur)
{
	int w = thread->inputImage.TellWidth();
	int h = thread->inputImage.TellHeight();
	vector<RGBApixel> pixels = {};

	for (int i = 0; i < w; i++)
	{
		for (int j = startBlur + 2; j < endBlur - 2; j++)
		{
			RGBApixel p1 = thread->inputImage.GetPixel(i, j - 2);
			RGBApixel p2 = thread->inputImage.GetPixel(i, j - 1);
			RGBApixel p3 = thread->inputImage.GetPixel(i, j);
			RGBApixel p4 = thread->inputImage.GetPixel(i, j + 1);
			RGBApixel p5 = thread->inputImage.GetPixel(i, j + 2);

			byte bR = (byte)((p1.Red + p2.Red + p3.Red + p4.Red + p5.Red) / 5);
			byte bG = (byte)((p1.Green + p2.Green + p3.Green + p4.Green + p5.Green) / 5);
			byte bB = (byte)((p1.Blue + p2.Blue + p3.Blue + p4.Blue + p5.Blue) / 5);

			RGBApixel bluredPixel;
			bluredPixel.Red = bR;
			bluredPixel.Green = bG;
			bluredPixel.Blue = bB;

			thread->outputImage->SetPixel(i, j, bluredPixel);
		}
	}
}

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	ThreadInfo* thread = static_cast<ThreadInfo*>(lpParam);
	int width = thread->inputImage.TellWidth();
	int height = thread->inputImage.TellHeight();
	int threadCount = thread->threadCount;
	int threadIndex = thread->index;
	clock_t startTime = thread->startTime;
	int startBlurIndex = (height / threadCount) * (threadIndex - 1);
	int endBlurIndex = (height / threadCount) * threadIndex;

	BlurProc(thread, startBlurIndex, endBlurIndex);
	ExitThread(0);
}

void CBlur::StarBluring() {
	BMP inputImage;
	BMP* outputImage = new BMP();
	if (!inputImage.ReadFromFile(m_inputImage)) { exit(0); };
	outputImage->ReadFromFile(m_inputImage);

	vector<vector<RGBApixel>> pixels = {};
	for (int i = 0; i < inputImage.TellWidth(); i++)
	{
		pixels.push_back({});
		for (int j = 0; j < inputImage.TellHeight(); j++)
		{
			pixels.at(0).push_back(inputImage.GetPixel(i, j));
		}
	}

	cout << "Opened " << m_inputImage << endl;
	cout << "Bluring..." << endl;
	vector<ThreadInfo*> threads = {};
	for (int i = 0; i < m_threadCount; ++i)
	{
		ThreadInfo* thread = new ThreadInfo();
		thread->index = i + 1;
		thread->inputImage = inputImage;
		thread->outputImage = outputImage;
		thread->threadCount = m_threadCount;
		thread->pixels = pixels;
		threads.push_back(thread);
	}
	vector<HANDLE> handles(threads.size());
	int affinityMask = (1 << m_coreCount) - 1;
	for (size_t i = 0; i < threads.size(); ++i)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, threads[i], CREATE_SUSPENDED, NULL);
		SetThreadPriority(handles.at(i), m_threadPriorities.at(i));
		SetThreadAffinityMask(handles[i], affinityMask);
	}

	for (const auto& handle : handles)
	{
		ResumeThread(handle);
	}

	WaitForMultipleObjects((DWORD)handles.size(), handles.data(), true, INFINITE);
	threads.at(0)->outputImage->WriteToFile(m_outputImage);
}

CBlur::CBlur(const char* inputImage, const char* outputImage, int threadCount, int coreCount, vector<int> threadPriorities)
	: m_inputImage(inputImage), m_outputImage(outputImage), m_threadCount(threadCount), m_coreCount(coreCount), m_threadPriorities(threadPriorities)
{}
