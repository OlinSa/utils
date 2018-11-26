#ifndef SRC_THREAD_MANAGER_H
#define SRC_THREAD_MANAGER_H

#include "job.h"
#include "thread_pool.h"

class CThreadManager
{
  public:
    CThreadManager()
    {
        num = 10;
        pool = new CThreadPool(num);
    }
    CThreadManager(int num)
    {
        this->num = num;
        pool = new CThreadPool(num);
    }
    virtual ~CThreadManager()
    {
        delete pool;
    }
    void Run(CJob *job, void *jobdata)
    {
        pool->Run(job, jobdata);
    }
    void TerminateAll(void)
    {
        pool->TerminateAll();
    }

  private:
    int num;
    CThreadPool *pool;
};

#endif