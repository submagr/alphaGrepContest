#include "node.h"
#include <cstddef>
#include <thread>
using namespace std;

template <class element>
node<element>::node(element newValue): value(newValue), next(NULL){
}

// Following is a shameless workaround
template class node<int>;
template class node<float>;
template class node<long>;
template class node<node<float> >;
template class node<thread *>;
