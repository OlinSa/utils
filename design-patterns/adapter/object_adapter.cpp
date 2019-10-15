#include <iostream>

using namespace std;

class Adaptee
{
public:
    void SpecialRequest()
    {
        cout<<"Adaptee: call SepcialRequest"<<endl;
    }
};

class Target
{
public:
    virtual void Request() = 0;
};

class ObjectApapter:public Target
{
public:
    ObjectApapter(Adaptee *adp):adapter(adp)
    {
    }
    void Request(){
        adapter->SpecialRequest();
    }
private:
    Adaptee *adapter;
};

int main(int argc, char *argv[])
{
    Adaptee *adaptee = new Adaptee();
    Target *objAdapter = new ObjectApapter(adaptee);
    objAdapter->Request();
    delete objAdapter;
    delete adaptee;
}