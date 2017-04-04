#ifndef __node_h__
#define __node_h__
using namespace std;

template <class element>
class node{
    public:
        element value;
        node* next;
        node(element newValue); 
};
#endif
