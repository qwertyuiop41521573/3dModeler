#ifndef ELEMENTCONTAINER_H
#define ELEMENTCONTAINER_H

#include <vector>

#include "vertex.h"
#include "triangle.h"

using namespace std;

class Journal;

//extended vector <T>
template <class T> class ElementContainer : public vector <T>
{
public:
    ElementContainer(Journal *journal);
    int push(const T &t);
    void remove(int index);
    void setSelected(int index, bool value);

    using vector <T>::size;
    using vector <T>::at;
    using vector <T>::push_back;

private:
    Journal *_journal;
};

template class ElementContainer <Vertex>;
template class ElementContainer <Triangle>;

#endif // ELEMENTCONTAINER_H
