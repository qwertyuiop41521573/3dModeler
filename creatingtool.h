#ifndef CREATINGTOOL_H
#define CREATINGTOOL_H

#include "toolwithwidget.h"

class CreatingTool : public ToolWithWidget
{
public:
    CreatingTool(MainWindow *mainWindow);

signals:

public slots:

protected:
    vector <int> ver;
    vector <int> tri;

    void removeAll();
};

#endif // CREATINGTOOL_H
