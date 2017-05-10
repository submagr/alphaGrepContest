#include "myqueue.h"
#include <cstddef>
#include <thread>
using namespace std;

template <class element>
myQueue<element>::myQueue(): head(NULL), tail(NULL){
}

template <class element>
myQueue<element>::~myQueue(){
    while(head!=NULL){
        node<element> *temp = head;
        head = head->next;
        delete(temp);
    }
}

template <class element>
bool myQueue<element>::isEmpty(){
    if(head == NULL)
        return true;
    else
        return false;
}

template <class element>
void myQueue<element>::push(element newValue){
    node<element> *newNode = new node<element>(newValue);
    if(isEmpty()){
        head = newNode;
        tail = head;
    }else{
        tail->next = newNode;
        tail = tail->next;
    }
}

template <class element>
void myQueue<element>::pop(){
    if(!isEmpty()){
        node<element>* poppedNode = head; 
        head = head->next;
        if(head ==NULL){
            tail =NULL;
        }
        delete(poppedNode);
    }
}

template <class element>
element myQueue<element>::front(){
    if(head!=NULL)
        return head->value;
}

// Following is a shameless workaround 
template class myQueue<int>;
template class myQueue<long>;
template class myQueue<node<float> >;
template class myQueue<thread *>;
