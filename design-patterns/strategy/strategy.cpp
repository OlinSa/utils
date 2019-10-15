#include <iostream>

using namespace std;

class Strategy
{
public:
    virtual void StrategyMethod() = 0;
};

class ConcreteStrategyA : public Strategy
{
public:
    void StrategyMethod()
    {
        cout << "strategyA is called" << endl;
    }
};

class ConcreteStrategyB : public Strategy
{
public:
    void StrategyMethod()
    {
        cout << "strategyB is called" << endl;
    }
};

class Context
{
public:
    Context() : strategy(nullptr) {}
    void SetStrategy(Strategy *st)
    {
        this->strategy = st;
    }
    Strategy *GetStrategy()
    {
        return strategy;
    }
    void StrategyMethod()
    {
        strategy->StrategyMethod();
    }

private:
    Strategy *strategy;
};

int main(int argc, char *argv[])
{
    Context *c = new Context();
    Strategy *stA = new ConcreteStrategyA();
    Strategy *stB = new ConcreteStrategyB();

    c->SetStrategy(stA);
    c->StrategyMethod();

    c->SetStrategy(stB);
    c->StrategyMethod();

    delete stA;
    delete stB;
    delete c;
}