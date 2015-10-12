#ifndef TPLANE_H
#define TPLANE_H

#include "creatingtool.h"

#include "gui/mycheckboxmw.h"

class TPlane : public CreatingTool
{
public:
    TPlane(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:
    
protected:
    MyCheckBoxMW *checkBoxSquare;

    bool planeFailed;

private:
    void countDiagonalForSquare(QVector2D *diagonal);
};

#endif // TPLANE_H
