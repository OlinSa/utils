#include <iostream>
using namespace std;

class Log
{
public:
    static Log *GetInstance()
    {
        static Log *instance = nullptr;
        if (!instance)
        {
            instance = new Log();
        }
        return instance;
    }
};

int main(int argc, char *argv[])
{
    Log *logInstance1 = Log::GetInstance();
    Log *logInstance2 = Log::GetInstance();
    cout << "log1 address:" << logInstance1 << endl;
    cout << "log2 address:" << logInstance2 << endl;

    delete logInstance1;
    delete logInstance2;
    return 0;
}