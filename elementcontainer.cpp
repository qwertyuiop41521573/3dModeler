#include "elementcontainer.h"
#include "journal.h"
#include "model.h"

#include <iostream>

using namespace std;

template <class T> ElementContainer <T>::ElementContainer(Journal *journal)
{
    _journal = journal;
}

template <class T> int ElementContainer <T>::push(const T &t)
{
    Model::modify();
    int index;
    int i;

    //find free place (some element marked as "not exists") for new element
    for(i = 0; i < size(); i++)
    {
        if(at(i).exists()) continue;
        
        at(i) = t;
        index = i;
        break;
    }

    //if no such place found
    if(i == size())
    {
        push_back(t);
        index = size() - 1;
    }

    //record to journal
    if(t.isVertex()) _journal->addVertex(index);
    else _journal->addTriangle(index);
    return index;
}


template <class T> void ElementContainer <T>::remove(int index)
{
    Model::modify();
    //"element.remove(i)" is the same as "element[i].remove()", but records to journal
    _journal->addBefore(at(0).isVertex(), index);
    at(index).remove();
    _journal->addAfter(at(0).isVertex());
}

template <class T> void ElementContainer <T>::setSelected(int index, bool value)
{
    //same as remove
    if(at(index).selected() == value) return;

    _journal->addBefore(at(0).isVertex(), index);
    at(index).setSelected(value);
    _journal->addAfter(at(0).isVertex());
}
