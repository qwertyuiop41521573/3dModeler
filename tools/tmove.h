#ifndef TMOVE_H
#define TMOVE_H

#include "toolwithwidget.h"
#include "gui/mypushbuttonmw.h"
#include "gui/myspinbox.h"

using namespace std;

class TMove : public ToolWithWidget
{
public:
    TMove(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MyPushButtonMW *pushButton[3];
    MySpinBox *spinBox[3];

    vector <bool> checked;
};

#endif // TMOVE_H
