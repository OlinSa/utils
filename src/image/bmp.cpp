#include "log.h"
#include "bmp.h"
#include <exception>

using namespace std;

CBmp::CBmp()
{
}
CBmp::~CBmp()
{
}
bool CBmp::LoadBmp(const std::string &f)
{
    this->filename = f;

    FILE *fp = fopen(f.c_str(), "rb");
    if (fp)
    {
        fread(&bmpHead, 1, sizeof(BITMAPFILEHEADER), fp);
        fread(&bmpInforHead, 1, sizeof(BITMAPINFODEADER), fp);
        fclose(fp);
        LOG_DEBUG("BITMAPFILEHEADER(%d Byte),BITMAPINFODEADER(%d Byte)", sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFODEADER));
    }
    else
    {
        LOG_ERR("open %s failed", f.c_str());
        throw exception();
    }
    return true;
}
void CBmp::ShowBmpHead()
{
    cout << "位图文件头:" << endl;
    cout << "文件头类型:" << bmpHead.bfType << endl;
    cout << "文件大小:" << bmpHead.bfSize << endl;
    cout << "保留字_1:" << bmpHead.bfReserved1 << endl;
    cout << "保留字_2:" << bmpHead.bfReserved2 << endl;
    cout << "实际位图数据的偏移字节数:" << bmpHead.bfOffBits << endl
         << endl;
}

void CBmp::ShowBmpInforHead()
{
    cout << "位图信息头:" << endl;
    cout << "结构体的长度:" << bmpInforHead.biSize << endl;
    cout << "位图宽:" << bmpInforHead.biWidth << endl;
    cout << "位图高:" << bmpInforHead.biHeight << endl;
    cout << "biPlanes平面数:" << bmpInforHead.biPlanes << endl;
    cout << "biBitCount采用颜色位数:" << bmpInforHead.biBitCount << endl;
    cout << "压缩方式:" << bmpInforHead.biCompression << endl;
    cout << "biSizeImage实际位图数据占用的字节数:" << bmpInforHead.biSizeImage << endl;
    cout << "X方向分辨率:" << bmpInforHead.biXPelsPerMeter << endl;
    cout << "Y方向分辨率:" << bmpInforHead.biYPelsPerMeter << endl;
    cout << "使用的颜色数:" << bmpInforHead.biClrUsed << endl;
    cout << "重要颜色数:" << bmpInforHead.biClrImportant << endl;
}
