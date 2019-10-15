#include <iostream>
#include <list>

using namespace std;

class Visitor;
class ConcreteElementA;
class ConcreteElementB;

class Element
{
public:
    Element() {}
    virtual ~Element() {}
    virtual void Accpet(Visitor *visitor) = 0;
};

class Visitor
{
public:
    Visitor() {}
    virtual ~Visitor() {}
    virtual void Visit(ConcreteElementA *ele) = 0;
    virtual void Visit(ConcreteElementB *ele) = 0;
};

class ConcreteElementA : public Element
{
public:
    void Accpet(Visitor *visitor)
    {
        visitor->Visit(this);
    }
    void OperationA()
    {
        cout << "call operationA()" << endl;
        return;
    }
};

class ConcreteElementB : public Element
{
public:
    void Accpet(Visitor *visitor)
    {
        visitor->Visit(this);
    }
    void OperationB()
    {
        cout << "call OperationB()" << endl;
        return;
    }
};

class ObjectStructure
{
public:
    void Accpet(Visitor *v)
    {
        list<Element *>::iterator it = l.begin();
        while (it != l.end())
        {
            (*it)->Accpet(v);
            it++;
        }
    }
    void Add(Element *e)
    {
        l.push_back(e);
    }
    void Remove(Element *e)
    {
        l.remove(e);
    }
    list<Element *> l;
};

class ConcreteVistorA : public Visitor
{
public:
    void Visit(ConcreteElementA *ele)
    {
        cout << "concreteVistorA call" << endl;
        ele->OperationA();
    }
    void Visit(ConcreteElementB *ele)
    {
        cout << "concreteVistorA call" << endl;
        ele->OperationB();
    }
};
class ConcreteVistorB : public Visitor
{
public:
    void Visit(ConcreteElementA *ele)
    {
        cout << "concreteVistorB call" << endl;
        ele->OperationA();
    }
    void Visit(ConcreteElementB *ele)
    {
        cout << "concreteVistorB call" << endl;
        ele->OperationB();
    }
};

int main(int argc, char *argv[])
{
    ObjectStructure *obj = new ObjectStructure();
    Element *e1 = new ConcreteElementA();
    Element *e2 = new ConcreteElementB();
    obj->Add(e1);
    obj->Add(e2);

    Visitor *vistor1 = new ConcreteVistorA();
    Visitor *vistor2 = new ConcreteVistorB();
    obj->Accpet(vistor1);
    obj->Accpet(vistor2);

    delete obj;
    delete e1;
    delete e2;
    delete vistor1;
    delete vistor2;
}