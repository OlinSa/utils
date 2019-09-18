#include <iostream>
#include <map>
#include <assert.h>

using namespace std;

class Cloneable
{
public:
    virtual Cloneable *Clone() = 0;
};

class Shape : public Cloneable
{
public:
    Shape()
    {
    }
    virtual ~Shape()
    {
    }
    virtual Shape *Clone() = 0;
    virtual double CountArea() = 0;
};

class Circle : public Shape
{
public:
    Circle *Clone()
    {
        return new Circle(*this);
    }
    double CountArea()
    {
        int r = 0;
        cout << "input radius:";
        cin >> r;
        return r * r * 3.1415926;
    }
};

class Square : public Shape
{
public:
    Shape *Clone()
    {
        return new Square(*this);
    }
    double CountArea()
    {
        int w = 0, h = 0;
        cout << "\ninput width:";
        cin >> w;
        cout << "\ninput height:";
        cin >> h;
        return w * h;
    }
};

class ProtoTypeManager
{
public:
    ProtoTypeManager()
    {
        AddShape("Circle", new Circle());
        AddShape("Square", new Square());
    }
    ~ProtoTypeManager()
    {
        map<string, Shape *>::iterator it = hp.begin();
        while (it != hp.end())
        {
            if (it->second)
            {
                delete it->second;
            }
            it++;
        }
    }
    void AddShape(string key, Shape *obj)
    {
        hp[key] = obj;
    }
    Shape *GetShape(string key)
    {
        Shape *shape = nullptr;
        if (hp.find(key) != hp.end())
        {
            shape = hp[key]->Clone();
        }
        return shape;
    }

private:
    map<string, Shape *> hp;
};

int main(int argc, char *argv[])
{
    ProtoTypeManager *pm = new ProtoTypeManager();
    Shape *circle = pm->GetShape("Circle");
    Shape *square = pm->GetShape("Square");

    assert(pm != nullptr);
    assert(circle != nullptr);
    assert(square != nullptr);

    double circleArea = circle->CountArea();
    double squareArea = square->CountArea();
    cout << "circle area:" << circleArea << endl;
    cout << "square area:" << squareArea << endl;

    delete pm;
    delete circle;
    delete square;
}