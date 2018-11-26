#ifndef SRC_JOB_H
#define SRC_JOB_H

#include <string.h>
#include <iostream>
#include "thread.h"

class CJob
{
  public:
    CJob() : jobno(0), jobname(NULL), workthread(NULL)
    {
    }
    virtual ~CJob()
    {
        if (jobname)
            free(jobname);
    }
    int GetJobNo()
    {
        return jobno;
    }
    void SetJobNo(int jobno)
    {
        this->jobno = jobno;
    }
    char *GetJobName(void)
    {
        return jobname;
    }
    void SetJobName(char *jobname)
    {
        this->jobname = strdup(jobname);
    }
    CThread *GetWorkThread()
    {
        return this->workthread;
    }
    void SetWorkThread(CThread *thread)
    {
        this->workthread = thread;
    }
    virtual void Run(void *ptr) = 0;

  private:
    int jobno;
    char *jobname;
    CThread *workthread;
};

#endif