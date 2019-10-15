#include <iostream>
#include <list>

using namespace std;

class Observer
{
public:
    virtual void Response(string msg) = 0;
};

class ConcreteObserver1 : public Observer
{
public:
    void Response(string msg)
    {
        cout << "notify observer1:" + msg << endl;
    }
};

class ConcreteObserver2 : public Observer
{
public:
    void Response(string msg)
    {
        cout << "notify observer2:" + msg << endl;
    }
};

class Subject
{
public:
    void Add(Observer *observer)
    {
        observers.push_back(observer);
    }
    void Remove(Observer *observer)
    {
        observers.remove(observer);
    }
    virtual void NotifyObserver(string msg)
    {
        list<Observer *>::iterator it;
        for (it = observers.begin(); it != observers.end(); it++)
        {
            (*it)->Response(msg);
        }
    }

private:
    list<Observer *> observers;
};

int main(int argc, char *argv[])
{
    Subject *sub = new Subject();
    Observer *obs1 = new ConcreteObserver1();
    Observer *obs2 = new ConcreteObserver2();
    sub->Add(obs1);
    sub->Add(obs2);

    sub->NotifyObserver("SIGHUP");

    delete sub;
    delete obs1;
    delete obs2;
}