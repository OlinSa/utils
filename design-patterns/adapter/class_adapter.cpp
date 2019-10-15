#include <iostream>

using namespace std;

class Target
{
public:
    virtual void Request() = 0;
};

class Adaptee
{
public:
    void SpecificRequest()
    {
        cout << "adaptee: call specificRequest" << endl;
    }
};

class ClassAdapter : public Adaptee, public Target
{
public:
    void Request()
    {
        SpecificRequest();
    }
};

int main(int argc, char *argvp[])
{
    Target *target = new ClassAdapter();
    target->Request();
    delete target;
}