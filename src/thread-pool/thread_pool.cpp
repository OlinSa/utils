#include <algorithm>
#include "log.h"
#include "thread_pool.h"
#include "worker_thread.h"
using namespace std;

CThreadPool::CThreadPool()
{
    init_num = idle_num = 10;
    CWorkerThread *thr;
    for (unsigned int i = 0; i < init_num; i++)
    {
        thr = new CWorkerThread();
        thr->SetThreadPool(this);
        thr->Start();
        AppendToIdleList(thr);
    }
}
CThreadPool::CThreadPool(int num) : init_num(num)
{
    init_num = idle_num = num;
    CWorkerThread *thr;
    for (unsigned int i = 0; i < init_num; i++)
    {
        thr = new CWorkerThread();
        thr->SetThreadPool(this);
        thr->Start();
        AppendToIdleList(thr);
    }
}
CThreadPool::~CThreadPool()
{
    TerminateAll();
}

void CThreadPool::TerminateAll()
{
    CWorkerThread *thr;
    for (unsigned int i = 0; i < thread_list.size(); i++)
    {
        thr = thread_list[i];
        thr->Join();
    }
    return;
}
void CThreadPool::Run(CJob *job, void *jobdata)
{
    CWorkerThread *idlethr = GetIdleThread();
    if (idlethr)
    {
        idlethr->SetThreadPool(this);
        job->SetWorkThread(idlethr);
        idlethr->SetJob(job, jobdata);
    }
}

void CThreadPool::AppendToIdleList(CWorkerThread *jobthread)
{
    mutex.Lock();
    idle_list.push_back(jobthread);
    thread_list.push_back(jobthread);
    mutex.UnLock();
}
void CThreadPool::MoveToBusyList(CWorkerThread *idlethread)
{
    mutex.Lock();
    busy_list.push_back(idlethread);

    vector<CWorkerThread *>::iterator pos = find(idle_list.begin(), idle_list.end(), idlethread);
    if (pos != idle_list.end())
    {
        idle_list.erase(pos);
    }
    idle_num--;
    mutex.UnLock();
}
void CThreadPool::MoveToIdleList(CWorkerThread *busythread)
{
    mutex.Lock();
    idle_list.push_back(busythread);

    vector<CWorkerThread *>::iterator pos = find(busy_list.begin(), busy_list.end(), busythread);
    if (pos != busy_list.end())
    {
        busy_list.erase(pos);
    }

    cond.Signal(); // wait up a blocking that is waiting for a idle thread

    idle_num++;
    mutex.UnLock();
}

CWorkerThread *CThreadPool::GetIdleThread()
{
    while (idle_list.size() == 0){
        cond.Wait();
    }
    CWorkerThread *idle = NULL;

    mutex.Lock();
    if(idle_list.size() > 0){
        idle = idle_list.front();
        LOG_DEBUG("get idle thread:%p", idle->GetThreadID());
    }

    mutex.UnLock();
    return idle;
}