#include "tls_wrap.h"
#include "log.h"

TLSWrap::TLSWrap()
{
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    ssl = NULL;
    sslCtx = NULL;
}
TLSWrap::~TLSWrap()
{
    if(ssl) {
        SSL_free(ssl);
    }
    if(sslCtx) {
        SSL_CTX_free(sslCtx);
    }
}

int TLSWrap::establish(
    const char *host,
    unsigned int port,
    const char *ca_crt ,
    size_t ca_crt_len)
{
    if(0 != TCPWrap::establish(host, port, ca_crt, ca_crt_len)) {
        return -1;
    }
}
int TLSWrap::destory()
{
}
int TLSWrap::getsockopt(int level, int optname, void *optval, socklen_t *optlen)
{

}
int TLSWrap::setsockopt(int level, int optname, void *optval, socklen_t optlen)
{
}
ssize_t TLSWrap::write(void *buf, ssize_t len)
{
}
ssize_t TLSWrap::read(void *buf, ssize_t count)
{
}
