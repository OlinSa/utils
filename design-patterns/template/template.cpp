#include <iostream>

using namespace std;

class AbstractClass
{
public:
    AbstractClass() {}
    virtual ~AbstractClass() {}
    void TemplateMethod()
    {
        SpecificMethod();
        AbstractMethod1();
        AbstractMethod2();
    }
    void SpecificMethod()
    {
        cout << "abstract special method" << endl;
    }
    virtual void AbstractMethod1() {}
    virtual void AbstractMethod2() {}
};

class ConcreteClass : public AbstractClass
{
public:
    void AbstractMethod1()
    {
        cout << "concrete method1 is called" << endl;
    }
    void AbstractMethod2()
    {
        cout << "concrete method2 is called" << endl;
    }
};

int main(int argc, char *argv[])
{
    AbstractClass *abs = new ConcreteClass();
    abs->TemplateMethod();
    delete abs;
    return 0;
}