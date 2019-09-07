#include <iostream>
#include "rb-tree.h"

using namespace std;

int main(int argc, char *argv[]) {
    int i;
    int a[] = {10, 40, 30, 60, 90, 70, 20, 50, 80};
    RBTree<int> *tree = new RBTree<int>();


    int iLen = sizeof(a) / sizeof(a[0]);
    cout << "origin array:";
    for (i = 0; i < iLen; i++) {
        cout << a[i] << " ";
    }
    cout << endl;

    for (i = 0; i < iLen; i++) {
        tree->Insert(a[i]);
    }

    cout << "PreOrder:" << endl;
    tree->PreOrder();
    cout<<endl;
    cout << "InOrder:" << endl;
    tree->InOrder();
    cout<<endl;
    cout << "PostOrder:" << endl;
    tree->PostOrder();
    cout<<endl;


    cout << "tree:Detail:" << endl;
    tree->Print();

    for (i = 0; i < iLen; i++) {
        tree->Remove(a[i]);
        cout << "remove " << a[i] << endl;
    }
    tree->Print();
    cout << endl;
    tree->Destory();

    delete tree;
}