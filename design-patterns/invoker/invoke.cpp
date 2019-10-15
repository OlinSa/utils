#include <iostream>

using namespace std;

class ReceiverA
{
public:
    void Action()
    {
        cout << "call reciverA Action()" << endl;
    }
};

class ReceiverB
{
public:
    void Action()
    {
        cout << "call reciverB Action()" << endl;
    }
};

class Command
{
public:
    Command() {}
    virtual ~Command() {}
    virtual void Execute() = 0;
};

class ConcreteCommandA : public Command
{
public:
    ConcreteCommandA()
    {
        receiver = new ReceiverA();
    }
    ~ConcreteCommandA()
    {
        delete receiver;
    }
    void Execute()
    {
        receiver->Action();
    }

private:
    ReceiverA *receiver;
};

class ConcreteCommandB : public Command
{
public:
    ConcreteCommandB()
    {
        receiver = new ReceiverB();
    }
    ~ConcreteCommandB()
    {
        delete receiver;
    }
    void Execute()
    {
        receiver->Action();
    }

private:
    ReceiverB *receiver;
};

class Invoker
{
public:
    Invoker(Command *cmd)
    {
        this->cmd = cmd;
    }
    void SetCommand(Command *cmd)
    {
        this->cmd = cmd;
    }

    void Call()
    {
        cmd->Execute();
    }

private:
    Command *cmd;
};

int main(int argc, char *argv[])
{
    Command *cmdA = new ConcreteCommandA();
    Command *cmdB = new ConcreteCommandB();
    Invoker *ir = new Invoker(cmdA);
    ir->Call();
    ir->SetCommand(cmdB);
    ir->Call();
    delete cmdA;
    delete cmdB;
    delete ir;
    return 0;
}