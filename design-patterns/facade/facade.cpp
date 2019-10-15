#include <iostream>

using namespace std;

class SubSystem01
{
public:
    SubSystem01() {}
    void Method()
    {
        cout << "call subsystem01 method" << endl;
    }
};

class SubSystem02
{
public:
    SubSystem02() {}
    void Method()
    {
        cout << "call subsystem02 method" << endl;
    }
};
class SubSystem03
{
public:
    SubSystem03() {}
    void Method()
    {
        cout << "call subsystem03 method" << endl;
    }
};

class Facade
{
public:
    Facade()
    {
        obj1 = new SubSystem01();
        obj2 = new SubSystem02();
        obj3 = new SubSystem03();
    }
    ~Facade()
    {
        delete obj1;
        delete obj2;
        delete obj3;
    }
    void Method()
    {
        obj1->Method();
        obj2->Method();
        obj3->Method();
    }

private:
    SubSystem01 *obj1;
    SubSystem02 *obj2;
    SubSystem03 *obj3;
};

int main(int argc, char *argv[])
{
    Facade *f = new Facade();
    f->Method();
    delete f;
}