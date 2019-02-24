#ifndef H264_WRAP_HH
#define H264_WRAP_HH

#include <string>
#include "log.h"

typedef enum
{
    NALU_TYPE_SLICE = 1,
    NALU_TYPE_DPA = 2,
    NALU_TYPE_DPB = 3,
    NALU_TYPE_DPC = 4,
    NALU_TYPE_IDR = 5,
    NALU_TYPE_SEI = 6,
    NALU_TYPE_SPS = 7,
    NALU_TYPE_PPS = 8,
    NALU_TYPE_AUD = 9,
    NALU_TYPE_EOSEQ = 10,
    NALU_TYPE_EOSTREAM = 11,
    NALU_TYPE_FILL = 12,
} NaluType;

typedef enum
{
    NALU_PRIORITY_DISPOSABLE = 0,
    NALU_PRIRITY_LOW = 1,
    NALU_PRIORITY_HIGH = 2,
    NALU_PRIORITY_HIGHEST = 3
} NaluPriority;

typedef struct
{
    int startCode; // 00 00 01 or 00 00 00 01
    int type;
    int size;
    unsigned char *data;
} NaluUnit;

/***
 *  NALU = Header + RBSP
 *  NALU Header = forbidden_zero_bit(1bit)+nal_ref_idc(2bits) +nalu__unit_type(5bit)
 *  RBSP = SODB(原始编码数据) + rbsp_stop_one_bit+rbsp_align_zero_bit
 */

class H264Wrap
{
  public:
    /**
    * @brief 从buffer中获取nalu
    *
    * @param [out] nalu: @n nalu 信息
    * @param [in] buf: @n buffer起始地址
    * @param [in] buf_size: @n buffer长度
    * @return 返回下一个nalu位置
    * @see None.
    * @note None.
    */
    int GetNaluFromBuffer(NaluUnit &nalu, unsigned char *buf, int buf_size);

    void PrintH264Info(std::string filename);
};

#endif