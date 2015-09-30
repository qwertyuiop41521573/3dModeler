#ifndef TROTATE_H
#define TROTATE_H

#include "toolwithwidget.h"

class TRotate : public ToolWithWidget
{
public:
    TRotate(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);

signals:

public slots:
};

#endif // TROTATE_H
