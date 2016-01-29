#ifndef TRIANGLECONTAINER_H
#define TRIANGLECONTAINER_H

#include <list>

#include "types.h"

using namespace std;

class Triangle;

class TriangleContainer : public list<Triangle>
{
public:
    tr_it push(const Triangle &triangle);
    void rem(tr_it it);
    void setSelected(tr_it it, bool value);
};

#endif // TRIANGLECONTAINER_H
