#ifndef ELEMENTCONTAINER_H
#define ELEMENTCONTAINER_H

#include <vector>

#include "vertex.h"
#include "triangle.h"

using namespace std;

template <class T> class ElementContainer : public vector <T>
{
public:
    int push(const T &t);
    void remove(int index);
    void setSelected(int index, bool value);

    using vector <T>::size;
    using vector <T>::at;
    using vector <T>::push_back;
};

template class ElementContainer <Vertex>;

#endif // ELEMENTCONTAINER_H
