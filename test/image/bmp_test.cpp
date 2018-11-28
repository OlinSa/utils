#include <iostream>
#include <unistd.h>
#include <string>
#include <exception>
#include "image/bmp.h"

using namespace std;

std::string GetCurProgName()
{
    char buf[1024] = {0};

    readlink("/proc/self/exe", buf, sizeof(buf));

    return std::string(buf);
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::string proName = GetCurProgName();
        std::cout << "Usage: " << proName << " filename" << endl;
        return -1;
    }
    try
    {
        string filename(argv[1]);
        CBmp bmp(filename);
        bmp.ShowBmpHead();
        bmp.ShowBmpInforHead();
    }
    catch(exception& e)
    {
        std::cout << e.what() << endl;
    }
}