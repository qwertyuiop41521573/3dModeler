#include "elementcontainer.h"
#include "journal.h"

template <class T> ElementContainer <T>::ElementContainer(Journal *journal)
{
    _journal = journal;
}

template <class T> int ElementContainer <T>::push(T t)
{
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
    _journal->add(index);
    return index;
}


template <class T> void ElementContainer <T>::remove(int index)
{
    _journal->add(index);
    at(index).remove();
}

