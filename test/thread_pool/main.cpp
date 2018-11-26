#include <iostream>
#include <list>
#include <unistd.h>
#include "thread_manager.h"
#include "job.h"

class MyJob : public CJob
{
  public:
    MyJob(){}
    ~MyJob(){}
    void Run(void *ptr)
    {
        std::cout<<"job task is run"<<std::endl;
        sleep(1);
    }
};

int main(int argc, char *argv[])
{
    CThreadManager *manager = new CThreadManager(3);
    std::list<MyJob*> job_list;
    MyJob* newjob;
    for(int i = 0; i < 40; i++){
        newjob = new MyJob();
        manager->Run(newjob, &i);
        job_list.push_back(newjob);
        // sleep(1);
    }

    sleep(10);
    manager->TerminateAll();

    std::list<MyJob*>::iterator pos;
    for(pos = job_list.begin(); pos != job_list.end(); pos++){
        delete *pos;
    }

    delete manager;
}