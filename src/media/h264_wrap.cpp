#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <unistd.h>

#include "h264_wrap.h"

using namespace std;

int H264Wrap::GetNaluFromBuffer(NaluUnit &nalu, unsigned char *buf, int buf_size)
{
    int i = 0;
    int startCodeCnt = 0;
    unsigned char *pbuf = buf;
    while (i < buf_size)
    {
        if (pbuf[i++] == 0x00 && pbuf[i++] == 0x00)
        {
            if (pbuf[i] == 0x01)
            {
                i += 1;
                startCodeCnt = 3;
            }
            else if (pbuf[i] == 0x00 && pbuf[i + 1] == 0x01)
            {
                i += 2;
                startCodeCnt = 4;
            }
            else
            {
                continue;
            }
        }
        else
        {
            continue;
        }

        //find next frame
        int pos = i;
        int startCodeCnt2 = 0;

        while (i < buf_size)
        {
            if (pbuf[i++] == 0x00 && pbuf[i++] == 0x00)
            {
                if (pbuf[i] == 0x01)
                {
                    i += 1;
                    startCodeCnt2 = 3;
                    break;
                }
                else if (pbuf[i] == 0x00 && pbuf[i + 1] == 0x01)
                {
                    i += 2;
                    startCodeCnt2 = 4;
                    break;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
        }

        nalu.startCode = startCodeCnt;
        if (i == buf_size)
        {
            nalu.size = i - pos;
        }
        else
        {
            nalu.size = (i - startCodeCnt2) - pos;
        }
        nalu.type = pbuf[pos] & 0x1f;
        nalu.data = &pbuf[pos];
        return i - startCodeCnt2;
    }
    return -1;
}

void H264Wrap::PrintH264Info(std::string filename)
{
    unsigned char buf[10240];
    int alreadSeen = 0;
    int left = sizeof(buf);

    int nextFramePos;
    std::streamsize readCount;

    NaluUnit nalu;

    ifstream inFile(filename, ios::in | ios::binary);
    if (!inFile)
    {
        LOG_DEBUG("open %s failed", filename);
        return;
    }
    while (inFile.read(reinterpret_cast<char *>(&buf[0]) + alreadSeen, left))
    {
        readCount = inFile.gcount();
        left -= readCount;
        alreadSeen += readCount;

        while ((nextFramePos = GetNaluFromBuffer(nalu, buf, alreadSeen)) > 0)
        {
            LOG_DEBUG("nalu:\nstartcode:%d,type:%d, data:%p, dataSize:%d,nextFramePos:%d",
                      nalu.startCode, nalu.type, nalu.data, nalu.size, nextFramePos);
            memmove(buf, buf + nextFramePos, alreadSeen - nextFramePos); //会有内存重叠问题
            left += nextFramePos;
            alreadSeen -= nextFramePos;
            for (int i = 0; i < 38; i++)
            {
                if (i % 8 == 0)
                {
                    cout << endl;
                }
                cout << "0x" << setfill('0') << setw(2) << hex << (buf[i] & 0xff) << " ";
            }
            cout << endl;
            sleep(1);
        }

        if (left == 0)
        {
            break;
        }
    }

    inFile.close();
    return;
}