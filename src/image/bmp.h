#ifndef SRC_IMAGE_BMP_H
#define SRC_IMAGE_BMP_H

#include <iostream>
#include <string>

#pragma pack(push)
#pragma pack(2)

typedef struct BITMAPFILEHEADER
{
    u_int16_t bfType;
    u_int32_t bfSize;
    u_int16_t bfReserved1;
    u_int16_t bfReserved2;
    u_int32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct BITMAPINFOHEADER
{
    u_int32_t biSize;
    u_int32_t biWidth;
    u_int32_t biHeight;
    u_int16_t biPlanes;
    u_int16_t biBitCount;
    u_int32_t biCompression;
    u_int32_t biSizeImage;
    u_int32_t biXPelsPerMeter;
    u_int32_t biYPelsPerMeter;
    u_int32_t biClrUsed;
    u_int32_t biClrImportant;
} BITMAPINFODEADER;

#pragma pack(pop)

class CBmp
{
  public:
    CBmp(const std::string &f);
    ~CBmp();
    void ShowBmpHead();
    void ShowBmpInforHead();

  private:
    std::string filename;
    BITMAPFILEHEADER bmpHead;
    BITMAPINFODEADER bmpInforHead;
};


#endif
