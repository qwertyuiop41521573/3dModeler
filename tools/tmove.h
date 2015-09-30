#ifndef TMOVE_H
#define TMOVE_H

#include "toolwithwidget.h"

class TMove : public ToolWithWidget
{
public:
    TMove(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);

signals:

public slots:
};

#endif // TMOVE_H
