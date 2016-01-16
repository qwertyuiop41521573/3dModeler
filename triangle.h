#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "element.h"

#include <QVector3D>

class Triangle : public Element
{
public:
    Triangle(int a = -1, int b = -1, int c = -1, int smoothingGroup = 0);
    Triangle(int *values, int smoothingGroup);
    void init(int smoothingGroup);

    int getIndex(int num) const
    { return index[num]; }

    void setIndex(int num, int newIndex)
    { index[num] = newIndex; }

    void operator =(const Triangle &triangle);

    int smoothingGroup() const { return _smoothingGroup; }
    QVector3D normal() const { return _normal; }
    void countNormal();

private:
    int index[3];
    int _smoothingGroup;
    QVector3D _normal;
};

#endif // TRIANGLE_H
