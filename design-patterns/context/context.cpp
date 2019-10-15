#include <iostream>

using namespace std;

class Context;

class State
{
public:
    virtual void Handle(Context *c) = 0;
};

class ConcreteStateA : public State
{
public:
    void Handle(Context *c)
    {
        cout << "current state:A" << endl;
    }
};

class ConcreteStateB : public State
{
public:
    void Handle(Context *c)
    {
        cout << "current state:B" << endl;
    }
};

class Context
{
public:
    void SetState(State *s)
    {
        this->state = s;
    }
    State *GetState()
    {
        return this->state;
    }
    void Handle()
    {
        state->Handle(this);
    }

private:
    State *state;
};

int main(int argc, char *argv[])
{
    State *stateA = new ConcreteStateA();
    State *stateB = new ConcreteStateB();
    Context *c = new Context();
    c->SetState(stateA);
    c->Handle();
    c->SetState(stateB);
    c->Handle();

    delete stateA;
    delete stateB;
    delete c;
    return 0;
}