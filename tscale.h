#ifndef TSCALE_H
#define TSCALE_H

#include "toolwithwidget.h"

class TScale : public ToolWithWidget
{
public:
    TScale(MainWindow *mainWindow,
           QWidget *widget = 0, WidgetElements *newElements = 0,
           QString finalButtonText = 0, bool hasStage2 = false);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);

signals:

public slots:
};

#endif // TSCALE_H
