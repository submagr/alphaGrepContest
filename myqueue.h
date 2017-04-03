#ifndef __myqueue_h__
#define __myqueue_h__
#include "node.h"
using namespace std;

class myQueue{
    public:
        node* head;
        node* tail;
        bool isEmpty();
        myQueue();
        ~myQueue();
        int front();
        void push(int newValue);
        void pop();
};
#endif
