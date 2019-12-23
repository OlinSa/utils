#include <iostream>

using namespace std;

void ConstCast() {
  const int a = 10;
  int *p1 = const_cast<int *>(&a);
  *p1 = 20;

  int &p2 = const_cast<int &>(a);
}

void StaticCast() {
  class Parent {
   public:
    Parent() { cout << "Parent construct" << endl; }
    ~Parent() { cout << "Parent Destruct" << endl; }
    void Show() { cout << "Parent Show" << endl; }
  };

  class Child : public Parent {
   public:
    Child() { cout << "Child construct" << endl; }
    ~Child() { cout << "Child Destruct" << endl; }
    void Print() { cout << "child show" << endl; }
  };

  float a = 1;
  int b = static_cast<int>(b);

  Parent par;
  Child ch;
  par.Show();
  ch.Print();
  par = static_cast<Parent>(ch);
  ch.Show();

  Parent *p1;
  p1 = static_cast<Parent *>(&ch);
  p1->Show();
}

void DynamicCast() {
  class Parent {
   public:
    Parent() { cout << "Parent construct" << endl; }
    ~Parent() { cout << "Parent Destruct" << endl; }
    virtual void Show() { cout << "Parent Show" << endl; }
  };

  class Child : public Parent {
   public:
    Child() { cout << "Child construct" << endl; }
    ~Child() { cout << "Child Destruct" << endl; }
    void Print() { cout << "child show" << endl; }
  };

  Parent *p1 = new Parent[10];
  Child *c1 = new Child[10];
  p1 = c1;
  p1->Show();
  c1 = dynamic_cast<Child *>(p1);
  c1->Show();
}

void ReinterpretCast() {
  float a = 1.23;

  int *b = reinterpret_cast<int *>(&a);
  cout << *b << endl;

  b = reinterpret_cast<int *>(0x100);
}

int main(int argc, char *argv[]) {
  ConstCast();
  StaticCast();
  DynamicCast();
  ReinterpretCast();
}