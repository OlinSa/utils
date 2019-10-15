#include <iostream>
#include <string>
#include <map>

using namespace std;

class UnsharedConcreteFlyweight
{
public:
    UnsharedConcreteFlyweight(string s) : info(s)
    {
    }
    void SetInfo(string info)
    {
        this->info = info;
    }
    string GetInfo()
    {
        return info;
    }

private:
    string info;
};

class Flyweight
{
public:
    virtual void Operation(UnsharedConcreteFlyweight *state) = 0;
};

class ConcreteFlyweight1 : public Flyweight
{
public:
    ConcreteFlyweight1(string key)
    {
        this->key = key;
        cout << "shared key:" + key + " was be created" << endl;
    }
    void Operation(UnsharedConcreteFlyweight *state)
    {
        cout << "shared key:" + key + " was be called" << endl;
        cout << "unshared info:" + state->GetInfo() << endl;
    }

private:
    string key;
};

class ConcreteFlyweight2 : public Flyweight
{
public:
    ConcreteFlyweight2(string key)
    {
        this->key = key;
        cout << "shared key:" + key + " was be created" << endl;
    }
    void Operation(UnsharedConcreteFlyweight *state)
    {
        cout << "shared key:" + key + " was be called" << endl;
        cout << "unshared info:" + state->GetInfo() << endl;
    }

private:
    string key;
};

class FlyweightFactory
{
public:
    Flyweight *GetFlyweight(string key)
    {
        Flyweight *flyweight = nullptr;
        map<string, Flyweight *>::iterator it = flyweights.find(key);
        if (it == flyweights.end())
        {
            flyweight = new ConcreteFlyweight1(key);
            flyweights[key] = flyweight;
        }
        else
        {
            flyweight = it->second;
            cout << "shared key:" + key + " was exists" << endl;
        }
        return flyweight;
    }

private:
    map<string, Flyweight *> flyweights;
};

int main(int argc, char *argv[])
{
    FlyweightFactory *facotory = new FlyweightFactory();
    Flyweight *f01 = facotory->GetFlyweight("a");
    Flyweight *f02 = facotory->GetFlyweight("a");
    Flyweight *f03 = facotory->GetFlyweight("a");
    Flyweight *f11 = facotory->GetFlyweight("b");
    Flyweight *f12 = facotory->GetFlyweight("b");
    UnsharedConcreteFlyweight *unshared01 = new UnsharedConcreteFlyweight("unshared01");
    UnsharedConcreteFlyweight *unshared02 = new UnsharedConcreteFlyweight("unshared02");
    UnsharedConcreteFlyweight *unshared03 = new UnsharedConcreteFlyweight("unshared03");
    f01->Operation(unshared01);
    f02->Operation(unshared02);
    f03->Operation(unshared03);
    f11->Operation(unshared01);
    f12->Operation(unshared02);

    delete facotory;
    delete f01;
    delete f11;
    delete unshared01;
    delete unshared02;
    delete unshared03;
}