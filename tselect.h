#ifndef TSELECT_H
#define TSELECT_H

#include "toolwithwidget.h"

class TSelect : public ToolWithWidget
{
public:
    TSelect(MainWindow *mainWindow, QString buttonText,
            QWidget *widget = 0, WidgetElements *newElements = 0,
            QString finalButtonText = 0, bool hasStage2 = false);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);

signals:

public slots:
};

#endif // TSELECT_H
