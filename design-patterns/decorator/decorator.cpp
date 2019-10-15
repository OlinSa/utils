#include <iostream>

using namespace std;

class Component
{
public:
    virtual void Operation() = 0;
};

class ConcreteComponent : public Component
{
public:
    void Operation()
    {
        cout << "ConcreteComponent: call Operation" << endl;
    }
};

class Decorator : public Component
{
public:
    Decorator(Component *component)
    {
        this->component = component;
    }
    void Operation()
    {
        component->Operation();
    }

private:
    Component *component;
};

class ConcreteDecorator1 : public Decorator
{
public:
    ConcreteDecorator1(Component *comp) : Decorator(comp)
    {
    }
    void Operation()
    {
        Decorator::Operation();
        AddedFunction();
    }
    void AddedFunction()
    {
        cout << "add extened fucnction1()" << endl;
    }
};

class ConcreteDecorator2 : public Decorator
{
public:
    ConcreteDecorator2(Component *comp) : Decorator(comp)
    {
    }
    void Operation()
    {
        Decorator::Operation();
        AddedFunction();
    }
    void AddedFunction()
    {
        cout << "add extened fucnction2()" << endl;
    }
};

int main(int argc, char *argv[])
{
    Component *component = new ConcreteComponent();
    Component *decorator = new ConcreteDecorator1(component);
    decorator->Operation();
    delete component;
    delete decorator;
}