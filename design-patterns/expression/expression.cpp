#include <iostream>
#include <set>
#include <vector>

using namespace std;

/**
 * rule:
 * <expression> :: = <city>|<persion>
 * <city> ::= nanjing | hanzhou
 * <persion> ::= old | woman | child
*/

class Expression
{
public:
    virtual bool Interpret(string info) = 0;
};

class TerminalExpression : public Expression
{
public:
    TerminalExpression(const string info[], int n)
    {
        for (int i = 0; i < n; i++)
        {
            s.insert(info[i]);
        }
    }
    bool Interpret(string info)
    {
        return s.find(info) != s.end();
    }

private:
    set<string> s;
};

class AndExpression : public Expression
{
public:
    AndExpression(Expression *city, Expression *person)
    {
        this->city = city;
        this->person = person;
    }

    bool Interpret(string info)
    {
        size_t pos = info.find('|');
        if (pos == string::npos)
            return false;

        string s1(info, 0, pos);
        string s2(info, pos + 1, string::npos);
        cout << "S1:" << s1 << endl;
        cout << "S2:" << s2 << endl;

        return city->Interpret(string(info, 0, pos)) && person->Interpret(string(info, pos + 1, string::npos));
    }

private:
    Expression *city;
    Expression *person;
};

class Context
{
public:
    Context()
    {
        string citys[] = {"hangzhou", "nanjing", "shanghai"};
        string persions[] = {"old", "woman"};
        city = new TerminalExpression(citys, 3);
        persion = new TerminalExpression(persions, 2);
        cityPersion = new AndExpression(city, persion);
    }
    virtual ~Context()
    {
        delete city;
        delete persion;
        delete cityPersion;
    }
    void FreeRide(string info)
    {
        if (cityPersion->Interpret(info))
        {
            cout << info << ":"
                 << "You are a cared for, you can travel for free" << endl;
        }
        else
        {
            cout << info << ":"
                 << "shoud pay 2$" << endl;
        }
    }

private:
    Expression *cityPersion;
    Expression *city;
    Expression *persion;
};

int main(int argc, char *argv[])
{
    Context *bus = new Context();
    bus->FreeRide("hangzhou|old");
    bus->FreeRide("hangzhou|young");
    bus->FreeRide("hangzhou|woman");
    bus->FreeRide("nanjing|old");
    bus->FreeRide("nanjing|young");
    bus->FreeRide("nanjing|woman");
    delete bus;
}