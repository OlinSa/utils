#include <getopt.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <malloc.h>
#include "log.h"
#include "io_wrap.h"
#include "rtmp_server.h"

typedef struct
{
    char *buf;
    int bufSize;
    char *cur;
    int pos;
} CACHE_CTX_T;

using namespace std;

static void Usage(char *appname)
{
    cout << appname << " [option]" << endl;
    cout << "option" << endl;
    cout << "\t"
         << "-f | --filename" << endl;
    cout << "\t"
         << "-u | --url" << endl;
    cout << "\t"
         << "-c | --cache" << endl;
}

static CACHE_CTX_T *createCache(int size)
{
    CACHE_CTX_T *cache = new CACHE_CTX_T();
    if (cache)
    {
        if ((cache->buf = new char[size]))
        {
            cache->bufSize = size;
            cache->cur = NULL;
            cache->pos = 0;
        }
        else
        {
            delete cache;
            cache = NULL;
        }
    }
    return cache;
}

static void releaseCache(CACHE_CTX_T *cache)
{
    if (cache)
    {
        delete[] cache->buf;
        delete cache;
    }
}

int main(int argc, char *argv[])
{
    LOG_SET_LEVEL(LOG_LEVEL_INFO);
    string url = "rtmp://127.0.0.1:1935/live/test264";
    int cacheSize = 10 * 1024;
    string filename;

    int c;
    const char *optstring = "f:u:c:";
    static struct option long_options[] = {
        {"filename", required_argument, 0, 'f'},
        {"url", optional_argument, 0, 'u'},
        {"cache", optional_argument, 0, 'c'}};
    int option_index;
    ;
    while (-1 != (c = getopt_long(argc, argv, optstring, long_options,
                                  &option_index)))
    {
        switch (c)
        {
        case 0:
            cout << "option " << long_options[option_index].name << " with arg"
                 << optarg << endl;
            break;
        case 'f':
            filename = optarg;
            break;
        case 'u':
            url = optarg;
            break;
        case 'c':
            cacheSize = atoi(optarg);
            break;
        default:
            cout << "?? get returned character code " << c << endl;
            Usage(argv[0]);
            return -1;
        }
    }
    cout << "filename:" << filename << endl;
    cout << "url:" << url << endl;
    cout << "cacheSize:" << cacheSize << endl;

    RtmpServer r;
    if (!r.Init(url, cacheSize))
    {
        cout << "rtmp init failed" << endl;
        return -1;
    }

    IOWrap ioWrap(filename);
    if (!ioWrap.Init())
    {
        cout << "iowrap init failed" << endl;
        return -1;
    }

    CACHE_CTX_T *cache = createCache(100 * 1024); //必须大于一个nalu,否则会影响解码
    if (!cache)
    {
        cout << "create cache failed" << endl;
        return -1;
    }

    int readSize = 0;
    int size;

    while ((readSize = ioWrap.Read(cache->buf + cache->pos, cache->bufSize - cache->pos)) > 0)
    {
        cache->pos += readSize;
        cache->cur = cache->buf;

        cout << "---------------" << endl;

        do
        {
            cout << "cur:" << (void *)cache->cur << " pos:" << cache->pos << endl;
            size = r.PublishH264(cache->cur, cache->pos, NULL);
            cout << "send " << size << " byte" << endl;
            if (size < 0)
            {
                break;
            }
            cache->cur += size;
            cache->pos -= size;
        } while (cache->pos > 0);
    }
    cout << "end!" << endl;

    releaseCache(cache);
    return 0;
}
