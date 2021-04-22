#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

const int THREAD_COUNT = 2;
const int OPERATION_COUNT = 2000000;
const int CYCLE_COUNT = 30;
const string OUTSTR = "output", TXTSTR = ".txt";

time_point<high_resolution_clock> startTime;

struct ThreadParams
{
    ofstream output;
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    ThreadParams* params = (ThreadParams*)lpParam;
    int temp;
    for (size_t i = 0; i < CYCLE_COUNT; i++)
    {
        for (int j = 0; j < OPERATION_COUNT; j++)
        {     
            temp = j + i * 2;
        }
        auto stopTime = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stopTime - startTime);
        params->output << to_string(duration.count()) << endl;
    }

    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    char ch;
    cin >> ch;

    startTime = high_resolution_clock::now();

    int threadCount = THREAD_COUNT;
    ThreadParams* paramsArray = new ThreadParams[threadCount];
    for (int i = 0; i < threadCount; i++)
    {
        paramsArray[i].output.open(OUTSTR + to_string(i) + TXTSTR);
    }

    // создание потоков
    HANDLE* handles = new HANDLE[threadCount];
    for (size_t i = 0; i < threadCount; i++)
    {
        handles[i] = CreateThread(NULL, 0, &ThreadProc, &paramsArray[i], CREATE_SUSPENDED, NULL);
    }

    // запуск потоков
    for (size_t i = 0; i < threadCount; i++)
    {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы потоков
    WaitForMultipleObjects(threadCount, handles, true, INFINITE);
    return 0;
}