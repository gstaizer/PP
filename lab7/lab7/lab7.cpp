#include <iostream>
#include <string>
#include "ThreadHandler.h"
#include "GetTask.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    if (argc != 2)
    {
        cout << "Enter Thread Count\n";
        return 0;
    }

    int threadCount = stoi(argv[1]);

    //�������� �������
    ITask** tasks = new ITask * [threadCount];
    for (size_t i = 0; i < threadCount; i++)
    {
        tasks[i] = new GetTask("����� �" + to_string(i + 1) + " ��������� ������ \n");
    }

    //������ �������
    ThreadHandler* handles = new ThreadHandler[threadCount];
    for (size_t i = 0; i < threadCount; i++)
    {
        handles[i].ExecuteTask(tasks[i]);
    }

    delete[] handles;
}