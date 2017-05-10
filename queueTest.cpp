#include<iostream>
#include "myqueue.h"
using namespace std;

int main(){
    myQueue<node<float> > *q = new myQueue<node<float> >();
    if(q->isEmpty()){
        cout << "q is empty initially" << endl;
    }
    q->pop();
    node<float> n1(4.0);
    node<float> n2(0.0);
    node<float> n3(1.0);
    n1.value = 4.0;
    n2.value = 0.0;
    n3.value = 1.0;
    q->push(n1);
    q->push(n2);
    q->push(n3);
    cout << (q->front()).value << endl;
    cout << (q->front()).value << endl;
    q->pop();
    cout << (q->front()).value << endl;
    q->pop();
    cout << (q->front()).value << endl;
    q->pop();
    q->pop();
    if(q->isEmpty()){
        cout << "q is empty at the end" << endl;
    }
    return 0;
}
