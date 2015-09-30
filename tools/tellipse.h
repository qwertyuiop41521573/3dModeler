#ifndef TELLIPSE_H
#define TELLIPSE_H

#include "toolwithwidget.h"

class TEllipse : public ToolWithWidget
{
public:
    TEllipse(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TELLIPSE_H
