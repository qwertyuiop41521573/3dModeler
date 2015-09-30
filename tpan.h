#ifndef TPAN_H
#define TPAN_H

#include "tool.h"

class TPan : public Tool
{
public:
    TPan(MainWindow *mainWindow, QString buttonText);

    void function(Action action, QMouseEvent
                  *event, VertexAndIndexData *data);

signals:

public slots:
};

#endif // TPAN_H
