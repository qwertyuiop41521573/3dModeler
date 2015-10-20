#ifndef TTRIANGLE_H
#define TTRIANGLE_H

#include "creatingtool.h"

class TTriangle : public CreatingTool
{
public:
    TTriangle(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);
    void setActive(bool value);

signals:

public slots:

private:
    vector <int> newTriangle;
};

#endif // TTRIANGLE_H
