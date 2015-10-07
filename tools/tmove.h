#ifndef TMOVE_H
#define TMOVE_H

#include "transformingtool.h"

#include "gui/mypushbuttonmw.h"

using namespace std;

class TMove : public TransformingTool
{
public:
    TMove(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MyPushButtonMW *pushButton[3];
};

#endif // TMOVE_H
