#ifndef SRC_THREAD_H
#define SRC_THREAD_H

#include <iostream>
#include <pthread.h>
#include "log.h"
#include "condition.h"
typedef enum
{
    THREAD_RUNNING = 0x0, //运行
    THREAD_IDLE = 0x1,    //空闲
    THREAD_EXIT = 0X2,    //退出
} ThreadState;

class CThread
{
  public:
    CThread()
    {
        threadid = -1;
        SetThreadState(THREAD_EXIT);
    }
    CThread(bool createsuspended, bool detach)
    {
        threadid = -1;
        SetThreadState(THREAD_EXIT);
    }
    virtual ~CThread()
    {
        SetThreadState(THREAD_EXIT);
    }

    pthread_t GetThreadID()
    {
        return threadid;
    }
    //do worker job
    virtual void Run() = 0;
    void SetThreadState(ThreadState state)
    {
        this->state = state;
    }
    ThreadState GetThreadState()
    {
        return this->state;
    }

    bool Terminate()
    {
        return true;
    }
    //start pthread
    bool Start()
    {
        bool ret = false;
        ret = pthread_create(&threadid, NULL, ThreadFunction, this);
        if (0 == ret)
        {
            LOG_DEBUG("create threadid:%p success", threadid)
            SetThreadState(THREAD_IDLE);
            ret = true;
        }
        return ret;
    }
    bool Exit()
    {
        SetThreadState(THREAD_EXIT);
        return true;
    }
    bool Detach()
    {
        pthread_detach(threadid);
        return true;
    }
    bool Join()
    {
        if (threadid > 0)
        {
            pthread_join(threadid, NULL);
        }
        return true;
    }

    pthread_t self()
    {
        return pthread_self();
    }

  private:
    static void *ThreadFunction(void *arg)
    {
        CThread *ptr =static_cast<CThread*>(arg);
        if(ptr){
            ptr->Run();
        }
        return NULL;
    }
    pthread_t threadid;
    ThreadState state;
    CCondition cond;
};

#endif