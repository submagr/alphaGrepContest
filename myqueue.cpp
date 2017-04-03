#include "myqueue.h"
#include <cstddef>

myQueue::myQueue(): head(NULL), tail(NULL){
}

myQueue::~myQueue(){
    while(head!=NULL){
        node *temp = head;
        head = head->next;
        delete(temp);
    }
}

bool myQueue::isEmpty(){
    if(head == NULL)
        return true;
    else
        return false;
}

void myQueue::push(int newValue){
    node *newNode = new node(newValue);
    if(isEmpty()){
        head = newNode;
        tail = head;
    }else{
        tail->next = newNode;
        tail = tail->next;
    }
}

void myQueue::pop(){
    if(!isEmpty()){
        node* poppedNode = head; 
        head = head->next;
        if(head ==NULL){
            tail =NULL;
        }
        delete(poppedNode);
    }
}

int myQueue::front(){
    if(head!=NULL)
        return head->value;
}
