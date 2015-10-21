#include "elementcontainer.h"
#include "journal.h"

#include <iostream>

using namespace std;

template <class T> ElementContainer <T>::ElementContainer(Journal *journal)
{
    _journal = journal;
}

template <class T> int ElementContainer <T>::push(const T &t)
{
    //cerr << t.isVertex();
    int index;
    int i;
    for(i = 0; i < size(); i++)
    {
        if(!at(i).exists())
        {
            at(i) = t;
            index = i;
            break;
        }
    }
    if(i == size())
    {
        push_back(t);
        index = size() - 1;
    }
    if(t.isVertex()) _journal->addVertex(index);
    else _journal->addTriangle(index);
    return index;
}


template <class T> void ElementContainer <T>::remove(int index)
{
    if(at(0).isVertex()) _journal->addVertex(index);
    else _journal->addTriangle(index);
    at(index).remove();
}

