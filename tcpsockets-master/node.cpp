#include "node.h"
#include <cstddef>
#include <thread>
using namespace std;

template <class element>
node<element>::node(element newValue): value(newValue), next(NULL){
}

template class node<int>;
template class node<float>;
template class node<node<float> >;
template class node<thread *>;
