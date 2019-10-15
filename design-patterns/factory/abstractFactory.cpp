#include <iostream>

using namespace std;

class Animal
{
public:
    Animal() {}
    virtual ~Animal() {}
    virtual void Show() = 0;
};
class Plant
{
public:
    Plant() {}
    virtual ~Plant() {}
    virtual void Show() = 0;
};

class Horse : public Animal
{
public:
    void Show()
    {
        cout << "horse" << endl;
    }
};
class Cattle : public Animal
{
public:
    void Show()
    {
        cout << "cattle" << endl;
    }
};
class Fruitage : public Plant
{
public:
    void Show()
    {
        cout << "fruitage" << endl;
    }
};
class Vegetables : public Plant
{
public:
    void Show()
    {
        cout << "vegetables" << endl;
    }
};

class Farm
{
public:
    Farm() {}
    virtual ~Farm() {}
    virtual Animal *NewAnimal() = 0;
    virtual Plant *NewPlant() = 0;
};

class SRFarm : public Farm
{
public:
    Animal *NewAnimal()
    {
        return new Horse();
    }
    Plant *NewPlant()
    {
        return new Fruitage();
    }
};

class SGFrame : public Farm
{
    Animal *NewAnimal()
    {
        return new Cattle();
    }
    Plant *NewPlant()
    {
        return new Vegetables();
    }
};

int main(int argc, char *argv[])
{
    Farm *srfarm = new SRFarm();
    Farm *sgfram = new SGFrame();
    Animal *horse = srfarm->NewAnimal();
    Animal *cattle = sgfram->NewAnimal();
    Plant *fruitage = srfarm->NewPlant();
    Plant *vegetables = sgfram->NewPlant();

    horse->Show();
    cattle->Show();
    fruitage->Show();
    vegetables->Show();

    delete srfarm;
    delete sgfram;
    delete horse;
    delete cattle;
    delete fruitage;
    delete vegetables;
}