#ifndef TCYLINDER_H
#define TCYLINDER_H

#include "tellipse.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TCylinder : public TEllipse
{
public:
    TCylinder(MainWindow *mainWindow);

       void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
       void createWallsAndSecondCap(QVector3D height);
};

#endif // TCYLINDER_H
