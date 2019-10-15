#include <iostream>

using namespace std;

class Implementor
{
public:
    virtual void OperationImpl() = 0;
};

class ConcreteImplementorA:public Implementor
{
public:
    void OperationImpl() 
    {
        cout<<"ConcreteImplementorA:call OperationImpl"<<endl;
    }
};

class ConcreteImplementorB:public Implementor
{
public:
    void OperationImpl() 
    {
        cout<<"ConcreteImplementorB:call OperationImpl"<<endl;
    }
};

class Abstraction
{
public:
    Abstraction(Implementor *imple)
    {
        this->imple = imple;
    }
    virtual void Operation() {};
protected:
    Implementor *imple;
};

class RefinedAbstraction:public Abstraction
{
public:
    RefinedAbstraction(Implementor *imple):Abstraction(imple)
    {
    }
    void Operation()
    {
        cout<<"Call refined Abstraction "<<endl;
        imple->OperationImpl();
    }
};

int main(int argc, char *argv[])
{
    Implementor *imple = new ConcreteImplementorA();
    Abstraction *abs = new RefinedAbstraction(imple);
    abs->Operation();
    delete imple;
    delete abs;
}