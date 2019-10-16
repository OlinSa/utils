#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

#include "udp_wrap.h"
#include "log.h"

int UDPWrap::establish(
    const char *host,
    unsigned int port,
    const char *ca_crt,
    size_t ca_crt_len)
{
    int ret = -1;
    char service[6];
    sprintf(service, "%u", port);

    struct addrinfo hints;
    struct addrinfo *cur = NULL;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    if (0 != getaddrinfo(host, service, &hints, &res))
    {
        LOG_ERR("getaddrinfo failed:%s", strerror(errno));
        return ret;
    }

    for (cur = res; cur != res; cur = cur->ai_next)
    {
        struct sockaddr_in *addr = (struct sockaddr_in *)cur->ai_addr;
        LOG_INFO("IP:%s", inet_ntoa(addr->sin_addr));

        if (cur->ai_family != AF_INET)
        {
            LOG_ERR("socket type error:%s", strerror(errno));
            continue;
        }
        fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
        if (fd < 0)
        {
            LOG_ERR("create socket error:%s", strerror(errno));
        }
        if (connect(fd, cur->ai_addr, cur->ai_addrlen) == 0)
        {
            LOG_INFO("connect %s:%u success", inet_ntoa(addr->sin_addr), port);
            ret = 0;
            break;
        }
        close(fd);
        LOG_ERR("connect failed:%s", strerror(errno));
    }
    freeaddrinfo(res);
    return ret;
}
int UDPWrap::destory()
{
    if (0 != shutdown(fd, 2))
    {
        LOG_ERR("shutdown failed:%s", strerror(errno));
        return -1;
    }
    if (0 != close(fd))
    {
        LOG_ERR("close socket failed:%s", strerror(errno));
        return -1;
    }
    return 0;
}
int UDPWrap::getsockopt(int level, int optname, void *optval, socklen_t *optlen)
{
    return ::getsockopt(fd, level, optname, optval, optlen);
}
int UDPWrap::setsockopt(int level, int optname, void *optval, socklen_t optlen)
{
    return ::setsockopt(fd, level, optname, optval, optlen);
}
ssize_t UDPWrap::write(void *buf, ssize_t len)
{
    return ::send(fd, buf, len, 0);
}
ssize_t UDPWrap::read(void *buf, ssize_t count)
{
    return ::recv(fd, buf, count, 0);
}