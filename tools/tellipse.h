#ifndef TELLIPSE_H
#define TELLIPSE_H

#include "creatingtool.h"

#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TEllipse : public CreatingTool
{
public:
    TEllipse();

    void function(Action action, QMouseEvent *event);

signals:

public slots:

protected:
    QSpinBox *spinBoxSegments;
    MySpinBox *spinBox[6];
    MySpinBox *spinBoxRadius;

    MyCheckBoxMW *checkBoxCircle;

    QVector3D normal;
    bool ellipseFailed;

private:
    void triangulateCap(bool flip = false);
    void createCap(QVector4D rotatingVertex, double angle, const QMatrix4x4 &scaleAndTranslate);

    QVector3D startPosition3D;
};

#endif // TELLIPSE_H
