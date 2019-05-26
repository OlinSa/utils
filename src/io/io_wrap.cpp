#include <log.h>
#include <stdarg.h>
#include "io_wrap.h"

using namespace std;

IOWrap::IOWrap()
    : cache(NULL)
{
}

IOWrap::IOWrap(std::string filename)
    : _filename(filename), cache(NULL)
{
}
bool IOWrap::Init()
{
    in.open(_filename, ios::in | ios::binary);
    if (!in)
    {
        cout << "open " << _filename << " failed" << endl;
        return false;
    }
    return true;
}
IOWrap::~IOWrap()
{
    if (in)
    {
        in.close();
    }
    if (cache)
    {
        delete[] cache->cache;
        delete[] cache;
    }
}

ssize_t IOWrap::Read(void *buf, size_t count)
{
    in.read(static_cast<char *>(buf), count);
    if (!in)
    {
        return -1;
    }
    return in.gcount();
}

ssize_t IOWrap::Read(data_proc_cb cb, void *ctx)
{
    ssize_t count = -1;
    if (!cache)
    {
        LOG_ERR("set io cache first");
        return -1;
    }

    in.read(static_cast<char *>(cache->cache), cache->cacheSize);
    if (!in)
    {
        return -1;
    }
    count = in.gcount();
    if (cb)
    {
        cb(cache->cache, count, ctx);
    }
    return count;
}

int IOWrap::Ioctl(unsigned long request, ...)
{
    int ret = -1;
    va_list ap;
    va_start(ap, request);
    switch (request)
    {
    case IOREQUEST_SETCACHE_SIZE:
    {
        int cacheSize = va_arg(ap, int);
        /* code */
        if (cache)
        {
            if (cacheSize == cache->cacheSize)
            {
                ret = 0;
                break;
            }
            delete[] cache->cache;
            cache->cacheSize = 0;
            delete[] cache;
            cache = NULL;
        }
        if (!cache)
        {
            cache = new IO_CACHE_T();
            if (!cache)
            {
                LOG_ERR("new IO_CACHE_T failed");
                ret = -1;
                break;
            }
            cache->cache = new char[cacheSize];
            if (!cache->cache)
            {
                LOG_ERR("new %d byte failed", cacheSize);
                delete[] cache;
                break;
            }
            ret = 0;
        }

        break;
    }
    case IOREQUEST_GETCACHE_SIZE:
    {
        /* code */
        if (cache)
        {
            ret = cache->cacheSize;
        }
        else
        {
            ret = 0;
        }
        break;
    }
    default:
    {
        LOG_ERR("don't support method:%lu ", request);
        break;
    }
    }

    va_end(ap);
    return ret;
}
