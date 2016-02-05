#include "vectuniqe.h"

template<class T> bool VectUniqe<T>::push(const T &t)
{
    int i;
    for(i = 0; i < size(); i++) if(at(i) == t) return false;
    if(i == size()) push_back(t);
    return true;
}
