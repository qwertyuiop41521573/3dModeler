#ifndef TTRIANGLE_H
#define TTRIANGLE_H

#include "toolwithwidget.h"

class TTriangle : public ToolWithWidget
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
