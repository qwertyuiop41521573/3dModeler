#ifndef TROTATE_H
#define TROTATE_H

#include "toolwithwidget.h"

class TRotate : public ToolWithWidget
{
public:
    TRotate(MainWindow *mainWindow,
            QWidget *widget = 0, WidgetElements *newElements = 0,
            QString finalButtonText = 0, bool hasStage2 = false);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);

signals:

public slots:
};

#endif // TROTATE_H
