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
    QCheckBox *checkBoxCube;
    MySpinBox *spinBox[6];

private slots:
    void handleCubeClick(bool value);
};

#endif // TBOX_H
