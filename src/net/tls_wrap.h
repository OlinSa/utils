#ifndef SRC_NET_TLS_WRAP_H
#define SRC_NET_TLS_WRAP_H

#include <unistd.h>
#include "tcp_wrap.h"
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"


class TLSWrap:public TCPWrap
{
public:
    TLSWrap();
    ~TLSWrap();
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

protected:
    SSL *ssl;
    SSL_CTX *sslCtx;
};


#endif