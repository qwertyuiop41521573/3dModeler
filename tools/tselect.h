#ifndef TSELECT_H
#define TSELECT_H

#include "toolwithwidget.h"

#include "gui/mycheckbox.h"

class TSelect : public ToolWithWidget
{
public:
    TSelect(MainWindow *mainWindow);

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MyCheckBox *checkBox[2];

    void countMinAndMax(QVector2D *min, QVector2D *max, const QVector2D &currentPosition);
};

#endif // TSELECT_H
