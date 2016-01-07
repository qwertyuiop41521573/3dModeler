#ifndef TVERTEX_H
#define TVERTEX_H

#include "creatingtool.h"

#include "gui/myspinbox.h"

class TVertex : public CreatingTool
{
public:
    TVertex();

    void function(Action action, QMouseEvent *event);
signals:

public slots:

private:
    MySpinBox *spinBox[3];
};

#endif // TVERTEX_H
