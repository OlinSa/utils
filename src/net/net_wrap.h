#ifndef SRC_NET_WRAP_H
#define SRC_NET_WRAP_H

#include <unistd.h>
#include <iostream>


class NetWrap
{
public:
    /**
     * @brief 建立连接结果
     *
     * @param[in] host: @n 服务器域名/ip
     * @param[in] port: @n 服务器端口
     * @param[in] ca_crt: @n root证书
     * @param[in] ca_crt_len: @n root证书长度
     *
     * @return 返回连接结果
     *
     * @retval < 0 : Fail.
     * @retval   0 : Success.
     */
    virtual int establish(
            const char *host,
            unsigned int port,
            const char *ca_crt = nullptr,
            size_t ca_crt_len = 0) = 0;
    /**
     * @brief 断开网络连接
     *
     * @return 返回结果
     *
     * @retval < 0 : Fail.
     * @retval   0 : Success.
     */
    virtual int destory() = 0;

    /**
     * @brief 获取任意类型、任意状态套接口的选项当前值，并把结果存入optval
     * 
     * @param[in] level: @n 选项定义的层次
     * @param[in] optname: @n 需获取的套接口选项
     * @param[in] optval: @n 指针，指向存放选项待设置的新值的缓冲区
     * @param[in] optlen: @n optval缓冲区长度
     * 
     * @return 返回结果
     *
     * @retval < 0 : Fail.
     * @retval   0 : Success.
     */
    virtual int getsockopt(int level, int optname, void *optval, socklen_t *optlen) = 0;

    /**
     * @brief 设置套接口的选项
     * 
     * @param[in] level: @n 选项定义的层次
     * @param[in] optname: @n 需获取的套接口选项
     * @param[in] optval: @n 指针，指向存放所获得选项值的缓冲区
     * @param[in] optlen: @n 指针，指向optval缓冲区的长度值
     * 
     * @return 返回结果
     *
     * @retval < 0 : Fail.
     * @retval   0 : Success.
     */
    virtual int setsockopt(int level, int optname, void *optval, socklen_t optlen) = 0;

    /**
     * @brief 向一个已经连接的socket发送数据，如果无错误，返回值为
     * 
     * @param[in] buf: @n 包含待发送数据的缓冲区
     * @param[in] len: @n 发送缓冲区长度
     * 
     * @return 所发送数据的总数
     */
    virtual ssize_t write(void *buf, ssize_t len) = 0;

    /**
     * @brief 从一个连接中接收数据
     * 
     * @param[in] buf: @n 接收缓冲区
     * @param[in] count: @n 接收缓冲区长度
     * 
     * @return 返回其实际copy的字节数
     */
    virtual ssize_t read(void *buf, ssize_t count) = 0;
protected:
    int fd;
};


#endif