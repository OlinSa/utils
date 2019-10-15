#include <iostream>
#include <vector>

using namespace std;

template <class Item>
class Iterator
{
public:
    Iterator(){};
    virtual ~Iterator(){};

    virtual void First() = 0;
    virtual void Next() = 0;
    virtual Item *CurItem() = 0;
    virtual bool IsDone() = 0;
};

template <class Item>
class Aggregate
{
public:
    Aggregate(){};
    virtual ~Aggregate(){};

    virtual void PushData(Item item) = 0;
    virtual Iterator<Item> *CreateIterator() = 0;
    virtual Item &operator[](int index) = 0;
    virtual int GetSize() = 0;
};

template <class Item>
class ConcreteIterator : public Iterator<Item>
{
public:
    ConcreteIterator(Aggregate<Item> *a) : aggr(a), cur(0){};
    virtual ~ConcreteIterator(){};

    virtual void First()
    {
        cur = 0;
    }
    virtual void Next()
    {
        if (cur < aggr->GetSize())
        {
            cur++;
        }
    }
    virtual Item *CurItem()
    {
        if (cur < aggr->GetSize())
        {
            return &(*aggr)[cur];
        }
        else
        {
            return nullptr;
        }
    }
    virtual bool IsDone()
    {
        return cur >= aggr->GetSize();
    }

private:
    Aggregate<Item> *aggr;
    int cur;
};

template <class Item>
class ConcreteAggregate : public Aggregate<Item>
{
public:
    void PushData(Item item)
    {
        data.push_back(item);
    }
    Iterator<Item> *CreateIterator()
    {
        return new ConcreteIterator<Item>(this);
    }
    Item &operator[](int index)
    {
        return data[index];
    }
    int GetSize()
    {
        return data.size();
    }

private:
    vector<Item> data;
};

int main(int argc, char *argv[])
{
    Aggregate<int> *aggr = new ConcreteAggregate<int>();
    aggr->PushData(3);
    aggr->PushData(2);
    aggr->PushData(1);
    Iterator<int> *it = aggr->CreateIterator();
    for (it->First(); !it->IsDone(); it->Next())
    {
        cout << *it->CurItem() << endl;
    }

    delete aggr;
    delete it;
}