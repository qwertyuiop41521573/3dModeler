#ifndef TSELECT_H
#define TSELECT_H

#include "toolwithwidget.h"

class TSelect : public ToolWithWidget
{
public:
    TSelect(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:
};

#endif // TSELECT_H
