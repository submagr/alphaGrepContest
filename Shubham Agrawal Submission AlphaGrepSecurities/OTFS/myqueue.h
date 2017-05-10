#ifndef __myqueue_h__
#define __myqueue_h__
#include "node.h"
using namespace std;

template <class element>
class myQueue{
    public:
        node<element>* head;
        node<element>* tail;
        bool isEmpty();
        myQueue();
        ~myQueue();
        element front();
        void push(element newValue);
        void pop();
};
#endif
