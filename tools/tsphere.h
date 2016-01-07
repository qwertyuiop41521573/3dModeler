#ifndef TSPHERE_H
#define TSPHERE_H

#include "creatingtool.h"

#include "gui/myspinbox.h"

class TSphere : public CreatingTool
{
public:
    TSphere();

    void function(Action action, QMouseEvent *event);

private:
    MySpinBox *spinBox[6];
    MySpinBox *spinBoxRadius;

    QSpinBox *spinBoxSegmentsXY;
    QSpinBox *spinBoxSegmentsZ;

    QVector3D startPosition3D;
    QVector3D normal;

    void triangulate();
    void setVertices(const QVector3D &center, double radius);
};

#endif // RSPHERE_H


    

