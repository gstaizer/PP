#include <windows.h>
#include <iostream>
#include <string>

CRITICAL_SECTION CriticalSection;

using namespace std;

void ValueAssignment(int value, int* workingVariable)
{
    *workingVariable = value;
    cout << to_string(*workingVariable) + ' ' + to_string(value) + "\n";
    if (*workingVariable != value)
    {
        cout << "Error \n";
    }
}

int GetValue(int& value)
{
    int delta = 0;
    delta = rand() % 10 + 1;
    value += delta;

    return value;
}

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    srand(GetCurrentThreadId());
    int* workingVariable = (int*)lpParam;
    int value = 0;

    for (size_t i = 0; i < 10; i++)
    {
        ValueAssignment(GetValue(value), workingVariable);
    }

    ExitThread(0); // функция устанавливает код завершения потока в 0
}

void RunThreads(int threadCount)
{
    int workingVariable = 1;

    HANDLE* handles = new HANDLE[threadCount];
    for (size_t i = 0; i < threadCount; i++)
    {
        handles[i] = CreateThread(NULL, 0, &ThreadProc, &workingVariable, CREATE_SUSPENDED, NULL);
    }

    for (size_t i = 0; i < threadCount; i++)
    {
        ResumeThread(handles[i]);
    }

    WaitForMultipleObjects(threadCount, handles, true, INFINITE);
}

DWORD WINAPI ThreadProcWithCS(CONST LPVOID lpParam)
{
    srand(GetCurrentThreadId());
    int* workingVariable = (int*)lpParam;
    int value = 0;

    for (size_t i = 0; i < 10; i++)
    {
        EnterCriticalSection(&CriticalSection);        
        ValueAssignment(GetValue(value), workingVariable);
        LeaveCriticalSection(&CriticalSection);
    }

    ExitThread(0); // функция устанавливает код завершения потока в 0
}

void RunThreadsWithCS(int threadCount)
{
    if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,
        0x00000400))
        return;

    int workingVariable = 1;

    HANDLE* handles = new HANDLE[threadCount];
    for (size_t i = 0; i < threadCount; i++)
    {
        handles[i] = CreateThread(NULL, 0, &ThreadProcWithCS, &workingVariable, CREATE_SUSPENDED, NULL);
    }

    for (size_t i = 0; i < threadCount; i++)
    {
        ResumeThread(handles[i]);
    }

    WaitForMultipleObjects(threadCount, handles, true, INFINITE);

    DeleteCriticalSection(&CriticalSection);
}

int main(int argc, char* argv[])
{
    char ch;
    cin >> ch;

    int threadCount = 2;
    cout << "Without CS: " << endl << endl;
    RunThreads(threadCount);

    cout << endl << "CS: " << endl << endl;
    RunThreadsWithCS(threadCount);

    return 0;
}