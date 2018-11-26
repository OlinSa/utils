#ifndef SRC_CONDITION_H
#define SRC_CONDITION_H

#include <pthread.h>

class CCondition
{
  public:
    CCondition()
    {
        cond = PTHREAD_COND_INITIALIZER;
        fastmutex = PTHREAD_MUTEX_INITIALIZER;
    }
    ~CCondition()
    {
    }
    void Wait()
    {
        pthread_mutex_lock(&fastmutex);
        pthread_cond_wait(&cond, &fastmutex);
        pthread_mutex_unlock(&fastmutex);
    }

    void Timedwait(struct timespec *abstime)
    {
        pthread_mutex_lock(&fastmutex);
        pthread_cond_timedwait(&cond, &fastmutex, abstime);
        pthread_mutex_unlock(&fastmutex);
    }
    void Signal()
    {
        pthread_mutex_lock(&fastmutex);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&fastmutex);
    }

    void Broadcast()
    {
        pthread_mutex_lock(&fastmutex);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&fastmutex);
    }

  private:
    pthread_cond_t cond;
    pthread_mutex_t fastmutex;
};

#endif