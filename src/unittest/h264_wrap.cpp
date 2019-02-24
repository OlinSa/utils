#include <iostream>
#include <string>
#include "media/h264_wrap.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return -1;
    }
    H264Wrap *h264_wrap = new H264Wrap();
    if (!h264_wrap)
    {
        cout << "new H264Wrap failed" << endl;
        return -1;
    }
    h264_wrap->PrintH264Info(string(argv[1]));
    delete h264_wrap;
    return 0;
}