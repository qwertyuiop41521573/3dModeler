#ifndef TROTATECAMERA_H
#define TROTATECAMERA_H

#include "tool.h"

class TRotateCamera : public Tool
{
public:
    TRotateCamera();

    void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TROTATECAMERA_H
