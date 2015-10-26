#ifndef TCYLINDER_H
#define TCYLINDER_H

#include "tellipse.h"

#include "gui/myspinbox.h"

class TCylinder : public TEllipse
{
public:
    TCylinder(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MySpinBox *spinBoxHeight;

    void createWallsAndSecondCap(bool final);
    void leave();
};

#endif // TCYLINDER_H
