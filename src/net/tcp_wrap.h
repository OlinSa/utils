#ifndef SRC_NET_TCP_WRAP_H
#define SRC_NET_TCP_WRAP_H

#include <unistd.h>
#include "net_wrap.h"

class TCPWrap:public NetWrap
{
public:
    virtual int establish(
            const char *host,
            unsigned int port,
            const char *ca_crt = nullptr,
            size_t ca_crt_len = 0);
    virtual int destory();
    virtual int getsockopt(int level, int optname, void *optval, socklen_t *optlen);
    virtual int setsockopt(int level, int optname, void *optval, socklen_t optlen);
    virtual ssize_t write(void *buf, ssize_t len);
    virtual ssize_t read(void *buf, ssize_t count);
};


#endif