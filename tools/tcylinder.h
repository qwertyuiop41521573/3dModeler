#ifndef TCYLINDER_H
#define TCYLINDER_H

#include "toolwithwidget.h"

class TCylinder : public ToolWithWidget
{
public:
    TCylinder(MainWindow *mainWindow);

       void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TCYLINDER_H
