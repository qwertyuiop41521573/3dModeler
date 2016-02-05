#ifndef VECTUNIQE_H
#define VECTUNIQE_H

#include "types.h"

#include <vector>

using namespace std;

template<class T> class VectUniqe : public vector<T>
{
public:
    bool push(const T &t);

    using vector<T>::operator =;
    using vector<T>::size;
    using vector<T>::push_back;
    using vector<T>::at;
};

template class VectUniqe<int>;
template class VectUniqe<tr_it>;

#endif // VECTUNIQE_H
