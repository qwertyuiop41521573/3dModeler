#ifndef TELLIPSE_H
#define TELLIPSE_H

#include "toolwithwidget.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TEllipse : public ToolWithWidget
{
public:
    TEllipse(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

protected:
    MySpinBox *spinBox[8];
    MyCheckBoxMW *checkBoxCircle;

    QVector3D normal;

    QVector3D createNormal(QVector3D camRot);

private:
    void allocateCap(bool flip = false);
    void createCap(QVector4D rotatingVertex, double angle, QVector3D normal,
                   QMatrix4x4 scaleAndTranslate);
    QMatrix4x4 createScaleAndTranslate(double scaleX, double scaleY, double
                                               scaleZ, QVector3D center);

    QVector3D startPosition3D;
};

#endif // TELLIPSE_H
