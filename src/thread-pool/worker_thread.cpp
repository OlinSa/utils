#include "worker_thread.h"
#include "thread_pool.h"

CWorkerThread::CWorkerThread()
{

}
CWorkerThread::~CWorkerThread()
{

}
void CWorkerThread::Run()
{
    SetThreadState(THREAD_RUNNING);
    for (;;)
    {
        while (job == NULL)
            job_cond.Wait(); //no job, wait it
        
        LOG_DEBUG("%p executable job", GetThreadID());
        job->Run(jobdata);
        LOG_DEBUG("%p done", GetThreadID());
        job->SetWorkThread(NULL);
        job = NULL;
        threadpool->MoveToIdleList(this);
    }
}
void CWorkerThread::SetJob(CJob *job, void *jobdata)
{
    var_mutex.Lock();
    this->job = job;
    this->jobdata = jobdata;
    job->SetWorkThread(this);
    var_mutex.UnLock();
    job_cond.Signal();
}
CJob *CWorkerThread::GetJob()
{
    return job;
}
void CWorkerThread::SetThreadPool(CThreadPool *pool)
{
    var_mutex.Lock();
    this->threadpool = pool;
    var_mutex.UnLock();
}
CThreadPool *CWorkerThread::GetThreadPool()
{
    return this->threadpool;
}