#include <iostream>

using namespace std;

class Subject
{
public:
    Subject() {}
    virtual ~Subject() {}
    virtual void Request() = 0;
};

class RealSubject : public Subject
{
public:
    void Request()
    {
        cout << "realsubject:request" << endl;
    }
};

class Proxy : public Subject
{
public:
    Proxy() : realSubject(nullptr)
    {
        if (realSubject == nullptr)
        {
            realSubject = new RealSubject();
        }
    }
    ~Proxy()
    {
        delete realSubject;
    }
    void PreRequest()
    {
        cout << "proxy:pre request" << endl;
    }
    void Request()
    {
        PreRequest();
        realSubject->Request();
        PostRequest();
    }
    void PostRequest()
    {
        cout << "proxy:post request" << endl;
    }

private:
    RealSubject *realSubject;
};

int main(int argc, char *argv[])
{
    Proxy *proxy = new Proxy();
    proxy->Request();
    delete proxy;
}