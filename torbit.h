#ifndef TORBIT_H
#define TORBIT_H

#include "tool.h"

class TOrbit : public Tool
{
public:
    TOrbit(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);

signals:

public slots:
};

#endif // TORBIT_H
