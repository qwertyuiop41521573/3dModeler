#ifndef TBOX_H
#define TBOX_H

#include "toolwithwidget.h"

class TBox : public ToolWithWidget
{
public:
    TBox(MainWindow *mainWindow);

  void function(Action action, QMouseEvent *event);


signals:

public slots:
};

#endif // TBOX_H
