#include <iostream>

using namespace std;

class Prototype
{
public:
    Prototype(int v) : value(v) {}
    virtual ~Prototype() {}
    virtual Prototype *clone() = 0;
    int value;
};

class Realizetype : public Prototype
{
public:
    Realizetype(int v) : Prototype(v)
    {
    }
    Realizetype *clone()
    {
        return new Realizetype(*this);
    }
};

int main(int argc, char *argv[])
{
    Prototype *obj1 = new Realizetype(10);
    Prototype *obj2 = obj1->clone();

    cout << "obj1 value:" << obj1->value << endl;
    cout << "obj2 value:" << obj2->value << endl;

    delete obj1;
    delete obj2;
}
