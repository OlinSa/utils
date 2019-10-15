#include <iostream>
#include <map>
#include <string>

using namespace std;

class Parlour
{
public:
    void SetWall(string wall)
    {
        this->wall = wall;
    }
    void SetTV(string tv)
    {
        this->tv = tv;
    }
    void SetSoft(string soft)
    {
        this->soft = soft;
    }
    void Show()
    {
        cout << "wall:" << wall << endl;
        cout << "tv:" << tv << endl;
        cout << "soft:" << soft << endl;
    }

private:
    string wall;
    ;
    string tv;
    string soft;
};

class Decorator
{
public:
    Decorator()
    {
        product = new Parlour();
    }
    virtual ~Decorator()
    {
    }
    virtual void BuildWall() = 0;
    virtual void BuildTV() = 0;
    virtual void BuildSoft() = 0;
    Parlour *GetResult()
    {
        return product;
    }

protected:
    Parlour *product;
};

class ConcreteDecorator1 : public Decorator
{
public:
    void BuildWall()
    {
        product->SetWall("wall1");
    }
    void BuildTV()
    {
        product->SetTV("tv1");
    }
    void BuildSoft()
    {
        product->SetSoft("soft1");
    }
};
class ConcreteDecorator2 : public Decorator
{
public:
    void BuildWall()
    {
        product->SetWall("wall2");
    }
    void BuildTV()
    {
        product->SetTV("tv2");
    }
    void BuildSoft()
    {
        product->SetSoft("soft2");
    }
};

class ProjectManager
{
public:
    ProjectManager(Decorator *builder)
    {
        this->builder = builder;
    }
    Parlour *Decorate()
    {
        builder->BuildWall();
        builder->BuildTV();
        builder->BuildSoft();
        return builder->GetResult();
    }

private:
    Decorator *builder;
};

int main(int argc, char *argv[])
{

    Decorator *d = new ConcreteDecorator2;
    ProjectManager *m = new ProjectManager(d);
    Parlour *p = m->Decorate();
    p->Show();

    delete d;
    delete m;
    delete p;
}