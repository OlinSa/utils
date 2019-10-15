#include <iostream>

using namespace std;

class Animal
{
public:
    Animal()
    {
    }
    virtual ~Animal()
    {
    }
    virtual void Show() = 0;
};

class Horse : public Animal
{
public:
    Horse()
    {
    }
    virtual ~Horse()
    {
    }
    void Show()
    {
        cout << "show horse" << endl;
    }
};

class Cattle : public Animal
{
public:
    Cattle()
    {
    }
    virtual ~Cattle()
    {
    }
    void Show()
    {
        cout << "show cattle" << endl;
    }
};
class AnimalFarm
{
public:
    AnimalFarm()
    {
    }
    virtual ~AnimalFarm()
    {
    }
    virtual Animal *NewAnimal() = 0;
};

class HorseFarm : public AnimalFarm
{
public:
    HorseFarm()
    {
    }
    virtual ~HorseFarm()
    {
    }
    Animal *NewAnimal()
    {
        return new Horse();
    }
};
class CattleFarm : public AnimalFarm
{
public:
    CattleFarm()
    {
    }
    virtual ~CattleFarm()
    {
    }
    Animal *NewAnimal()
    {
        return new Cattle();
    }
};

int main(int argc, char *argv[])
{
    AnimalFarm *horsefarm = new HorseFarm();
    AnimalFarm *cattlefarm = new CattleFarm();
    Animal *horse = horsefarm->NewAnimal();
    Animal *cattle = cattlefarm->NewAnimal();
    horse->Show();
    cattle->Show();

    delete horse;
    delete cattle;
    delete horsefarm;
    delete cattlefarm;
}