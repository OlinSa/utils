#ifndef SRC_MUTEX_H
#define SRC_MUTEX_H

#include <pthread.h>

class CMutex
{
public:
    CMutex()
    {
        fastmutex = PTHREAD_MUTEX_INITIALIZER;
    }
    ~CMutex()
    {

    }
    bool Lock()
    {
        return pthread_mutex_lock(&fastmutex) == 0 ? true:false;
    }
    bool UnLock()
    {
        return pthread_mutex_unlock(&fastmutex) == 0 ? true:false;
    }
    bool TryLock()
    {
        return pthread_mutex_trylock(&fastmutex) == 0 ? true:false;
    }
private:
    pthread_mutex_t fastmutex;
};

#endif