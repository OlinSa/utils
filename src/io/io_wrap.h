#ifndef IOWRAP_HH
#define IOWRAP_HH

#include <iostream>
#include <string>
#include <fstream>


#define IOREQUEST_SETCACHE_SIZE (0x01)
#define IOREQUEST_GETCACHE_SIZE (0x02)

typedef struct {
    char *cache;
    int cacheSize;
}IO_CACHE_T;


//消息处理函数
typedef ssize_t (*data_proc_cb)(const void *buf, ssize_t count, void *ctx);

class IOWrap{
public:
    IOWrap();
    IOWrap(std::string filename);
    bool Init();
    ~IOWrap();

    /**
    * @brief 从源读取数据
    *
    * @param [in] buf: @n 数据输出存储地址
    * @param [in] buf: @n buf size
    * @return 读取的数据长度
    * @see None.
    * @note None.
    */
    ssize_t Read(void *buf, size_t count);
    /**
    * @brief 从源读取数据
    *
    * @param [in] cb: @n 数据处理函数
    * @param [in] ctx: @n 环境变量
    * @return 读取的数据长度
    * @see None.
    * @note None.
    */
    ssize_t Read(data_proc_cb cb, void *ctx);


    int Ioctl(unsigned long request, ...);

private:
    std::ifstream in;
    std::string _filename;
    IO_CACHE_T *cache;
};


#endif