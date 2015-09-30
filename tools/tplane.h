#ifndef TPLANE_H
#define TPLANE_H

#include "toolwithwidget.h"

class TPlane : public ToolWithWidget
{
public:
    TPlane(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);

signals:

public slots:
};

#endif // TPLANE_H
