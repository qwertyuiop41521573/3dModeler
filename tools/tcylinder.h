#ifndef TCYLINDER_H
#define TCYLINDER_H

#include "tellipse.h"

class TCylinder : public TEllipse
{
public:
    TCylinder(MainWindow *mainWindow);

       void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
       void createWallsAndSecondCap(const QVector3D &height);
};

#endif // TCYLINDER_H
