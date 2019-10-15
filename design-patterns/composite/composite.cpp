#include <iostream>
#include <list>
using namespace std;

class Component
{
public:
    virtual void Add(Component *c) = 0;
    virtual void Remove(Component *c) = 0;
    virtual Component *GetChild(int c) = 0;
    virtual void Operation() = 0;
};

class Leaf : public Component
{
public:
    Leaf(string name)
    {
        this->name = name;
    }
    void Add(Component *c)
    {
    }
    virtual void Remove(Component *c)
    {
    }
    virtual Component *GetChild(int c)
    {
        return nullptr;
    }
    virtual void Operation()
    {
        cout << "leaf " + name + " was be call" << endl;
    }

private:
    string name;
};

class Composite : public Component
{
public:
    void Add(Component *c)
    {
        children.push_back(c);
    }
    void Remove(Component *c)
    {
        list<Component *>::iterator it;
        for (it = children.begin(); it != children.end(); ++it)
        {
            if (*it == c)
            {
                children.erase(it);
                break;
            }
        }
    }
    Component *GetChild(int c)
    {
        int i = 0;
        list<Component *>::iterator it;
        for (it = children.begin(); it != children.end(); ++it)
        {
            if (i++ == c)
            {
                return *it;
            }
        }
        return nullptr;
    }
    void Operation()
    {
        list<Component *>::iterator it;
        for (it = children.begin(); it != children.end(); ++it)
        {
            (*it)->Operation();
        }
    }

private:
    list<Component *> children;
};

int main(int argc, char *argv[])
{
    Component *c0 = new Composite();
    Component *c1 = new Composite();

    Component *leaf1 = new Leaf("1");
    Component *leaf2 = new Leaf("2");
    Component *leaf3 = new Leaf("3");

    c0->Add(leaf1);
    c0->Add(c1);
    c1->Add(leaf2);
    c1->Add(leaf3);
    c0->Operation();

    delete c0;
    delete c1;
    delete leaf1;
    delete leaf2;
    delete leaf3;
}