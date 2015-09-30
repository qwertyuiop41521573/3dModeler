#ifndef TROTATECAMERA_H
#define TROTATECAMERA_H

#include "tool.h"

class TRotateCamera : public Tool
{
public:
    TRotateCamera(MainWindow *mainWindow, QString buttonText);

    void function(Action action, QMouseEvent
                  *event, VertexAndIndexData *data);

signals:

public slots:
};

#endif // TROTATECAMERA_H
