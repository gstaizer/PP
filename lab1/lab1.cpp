#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	cout << "Поток №" + to_string(*(int*)lpParam) + " выполняет работу \n";
	ExitThread(0); // функция устанавливает код завершения потока в 0
}


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "ru");
	
	if (argc != 2)
	{
		return 1;
	}

	int threadCount = stoi(argv[1]);
	HANDLE* handles = new HANDLE[threadCount];
	for (int i = 0; i < threadCount; i++)
	{
		handles[i] = CreateThread(NULL, 0, &ThreadProc, new int(i + 1), CREATE_SUSPENDED, NULL);
		ResumeThread(handles[i]);
	}
	WaitForMultipleObjects(threadCount, handles, true, INFINITE);
	return 0;
}
