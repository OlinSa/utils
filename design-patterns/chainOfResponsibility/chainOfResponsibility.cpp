#include <iostream>

using namespace std;

class Handler
{
public:
    Handler() : next(nullptr) {}
    void SetNext(Handler *next)
    {
        this->next = next;
    }
    Handler *GetNext()
    {
        return this->next;
    }
    virtual void HandleRequest(string request) = 0;

private:
    Handler *next;
};

class ConcreteHandler1 : public Handler
{
public:
    void HandleRequest(string request)
    {
        if (request == "one")
        {
            cout << "concrete handler1 do it" << endl;
        }
        else
        {
            if (GetNext())
            {
                GetNext()->HandleRequest(request);
            }
            else
            {
                cout << "no one handle " + request << endl;
            }
        }
    }
};

class ConcreteHandler2 : public Handler
{
public:
    void HandleRequest(string request)
    {
        if (request == "two")
        {
            cout << "concrete handler2 do it" << endl;
        }
        else
        {
            if (GetNext())
            {
                GetNext()->HandleRequest(request);
            }
            else
            {
                cout << "no one handle " + request << endl;
            }
        }
    }
};

int main(int argc, char *argv[])
{
    Handler *handler1 = new ConcreteHandler1();
    Handler *handler2 = new ConcreteHandler2();

    handler1->SetNext(handler2);

    handler1->HandleRequest("two");
    handler1->HandleRequest("three");

    delete handler1;
    delete handler2;
}