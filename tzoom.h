#ifndef TZOOM_H
#define TZOOM_H

#include "tool.h"

class TZoom : public Tool
{
public:
    TZoom(MainWindow *mainWindow, QString buttonText);

    void function(Action action, QMouseEvent
                  *event, VertexAndIndexData *data);

signals:

public slots:
};

#endif // TZOOM_H
