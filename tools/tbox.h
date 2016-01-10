#ifndef TBOX_H
#define TBOX_H

#include "tools/tplane.h"

#include "gui/myspinbox.h"

class TBox : public TPlane
{
    Q_OBJECT
public:
    TBox();

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    QCheckBox *checkBoxCube;
    MySpinBox *spinBox[6];

    void leave();

private slots:
    void handleCubeClick(bool value);
};

#endif // TBOX_H
