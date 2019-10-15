#include <iostream>

using namespace std;

class Memento
{
public:
    Memento(string state)
    {
        this->state = state;
    }
    void SetState(string state)
    {
        this->state = state;
    }
    string GetState()
    {
        return this->state;
    }

private:
    string state;
};

class Caretaker
{
public:
    void SetMemento(Memento *m)
    {
        this->memento = m;
    }
    Memento *GetMemento()
    {
        return this->memento;
    }

private:
    Memento *memento;
};

class Originator
{
public:
    void SetState(string state)
    {
        this->state = state;
    }
    string GetState()
    {
        return this->state;
    }
    Memento *CreateMemento()
    {
        return new Memento(state);
    }
    void RestoreMemento(Memento *m)
    {
        this->SetState(m->GetState());
    }

private:
    string state;
};

int main(int argc, char *argv[])
{
    Originator *ori = new Originator();
    Caretaker *cr = new Caretaker();

    ori->SetState("S0");
    cout << "origin:" << ori->GetState() << endl;

    Memento *temp = ori->CreateMemento();
    cr->SetMemento(temp);
    ori->SetState("S1");
    cout << "new:" << ori->GetState() << endl;

    ori->RestoreMemento(cr->GetMemento());
    cout << "restore:" << ori->GetState() << endl;

    delete ori;
    delete cr;
    delete temp;
}