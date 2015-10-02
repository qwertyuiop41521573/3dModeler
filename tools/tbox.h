#ifndef TBOX_H
#define TBOX_H

#include "tools/tplane.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"
#include "gui/mylabel.h"

class TBox : public TPlane
{
    Q_OBJECT
public:
    TBox(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MySpinBox *spinBox[6];
    QCheckBox *checkBoxCube;

    void countDiagonalForSquare(QVector2D *diagonal);

private slots:
    void h(bool q) {};
};

#endif // TBOX_H
