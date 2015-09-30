#ifndef TMOVE_H
#define TMOVE_H

#include "toolwithwidget.h"

class TMove : public ToolWithWidget
{
public:
    TMove(MainWindow *mainWindow,
          QWidget *widget = 0, WidgetElements *newElements = 0,
          QString finalButtonText = 0, bool hasStage2 = false);

    void function(Action action, QMouseEvent *event,
                  VertexAndIndexData *data);

signals:

public slots:
};

#endif // TMOVE_H
