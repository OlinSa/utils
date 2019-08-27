#include <iostream>

using namespace std;

void a(){
    cout<<"\t\t+---call a() function"<<endl;
}

void c(){
    cout<<"\t\t+---call c() function"<<endl;
}

int b() {
    cout<<"\t+--- call b() function"<<endl;
    a();
    c();
    return 0;
}


int main(int argc, char *argv[])
{
    b();
}