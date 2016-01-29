#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "element.h"

#include <QVector3D>

class Vertex;

class Triangle : public Element
{
public:
    Triangle(int a = -1, int b = -1, int c = -1, int smoothingGroup = 0);
    Triangle(int *values, int smoothingGroup);
    Triangle(const Triangle &triangle);

    int getIndex(int num) const { return index[num]; }
    void setIndex(int num, int newIndex);

    void operator =(const Triangle &triangle);

    int smoothingGroup() const { return _smoothingGroup; }
    QVector3D normal() const { return _normal; }
    void countNormal();
    const Vertex &vertex(int ind) const;
    Vertex &vertex(int ind);

    void remove();
    void record();

private:
    int index[3];
    int _smoothingGroup;
    QVector3D _normal;

    void init(int smoothingGroup);
};

#endif // TRIANGLE_H
