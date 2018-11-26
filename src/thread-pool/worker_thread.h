#ifndef SRC_WORKTHREAD_H
#define SRC_WORKTHREAD_H

#include "job.h"
#include "thread.h"
#include "condition.h"
#include "mutex.h"

class CThreadPool;

class CWorkerThread:public CThread
{
public:
    CWorkerThread();
    virtual ~CWorkerThread();
    void Run();
    void SetJob(CJob *job, void *jobdata);
    CJob *GetJob();
    void SetThreadPool(CThreadPool *pool);
    CThreadPool *GetThreadPool();

private:
    CJob *job;
    void *jobdata;
    CCondition job_cond;
    CMutex var_mutex;
    CThreadPool *threadpool;
};


#endif