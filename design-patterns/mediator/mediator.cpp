#include <iostream>
#include <list>

using namespace std;

class Mediator;

class Colleague
{
public:
    virtual void SetMediator(Mediator *mediator)
    {
        this->mediator = mediator;
    }
    virtual void Send() = 0;
    virtual void Receive() = 0;

protected:
    Mediator *mediator;
};

class Mediator
{
public:
    virtual void Register(Colleague *c) = 0;
    virtual void Replay(Colleague *c) = 0;
};

class Colleague1 : public Colleague
{
public:
    void Send()
    {
        cout << "colleague1 send msg" << endl;
        mediator->Replay(this);
    }
    void Receive()
    {
        cout << "colleague1 receive msg" << endl;
    }
};
class Colleague2 : public Colleague
{
public:
    void Send()
    {
        cout << "colleague2 send msg" << endl;
        mediator->Replay(this);
    }
    void Receive()
    {
        cout << "colleague2 receive msg" << endl;
    }
};

class ConcreteMediator : public Mediator
{
public:
    void Register(Colleague *c)
    {
        list<Colleague *>::iterator it = colleagues.begin();
        while (it != colleagues.end())
        {
            if (*(it) == c)
            {
                return;
            }
            it++;
        }
        colleagues.push_back(c);
        c->SetMediator(this);
    }
    void Replay(Colleague *c)
    {
        for (list<Colleague *>::iterator it = colleagues.begin(); it != colleagues.end(); it++)
        {
            if ((*it) != c)
            {
                (*it)->Receive();
            }
        }
    }

private:
    list<Colleague *> colleagues;
};

int main(int argc, char *argv[])
{
    Mediator *md = new ConcreteMediator();

    Colleague *c1 = new Colleague1();
    Colleague *c2 = new Colleague2();

    md->Register(c1);
    md->Register(c2);
    c1->Send();
    cout << "----" << endl;
    c2->Send();

    delete md;
    delete c1;
    delete c2;
}