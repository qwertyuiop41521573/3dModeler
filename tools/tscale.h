#ifndef TSCALE_H
#define TSCALE_H

#include "toolwithwidget.h"

class TScale : public ToolWithWidget
{
public:
    TScale(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TSCALE_H
