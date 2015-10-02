#ifndef TVERTEX_H
#define TVERTEX_H

#include "toolwithwidget.h"
#include "gui/myspinbox.h"

class TVertex : public ToolWithWidget
{
public:
    TVertex(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);
signals:

public slots:

private:
    MySpinBox *spinBox[3];
};

#endif // TVERTEX_H
