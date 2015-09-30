#ifndef TVERTEX_H
#define TVERTEX_H

#include "toolwithwidget.h"

class TVertex : public ToolWithWidget
{
public:
    TVertex(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);
signals:

public slots:
};

#endif // TVERTEX_H
