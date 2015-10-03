#ifndef TPLANE_H
#define TPLANE_H

#include "toolwithwidget.h"
#include "gui/myspinbox.h"
#include "gui/mycheckboxmw.h"

class TPlane : public ToolWithWidget
{
public:
    TPlane(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:
    
protected:
    MyCheckBoxMW *checkBoxSquare;

    void countDiagonalForSquare(QVector2D *diagonal);

private:
};

#endif // TPLANE_H
