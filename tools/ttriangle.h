#ifndef TTRIANGLE_H
#define TTRIANGLE_H

#include "toolwithwidget.h"

class TTriangle : public ToolWithWidget
{
public:
    TTriangle(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TTRIANGLE_H
