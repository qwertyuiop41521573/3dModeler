#ifndef TSELECT_H
#define TSELECT_H

#include "toolwithwidget.h"

#include "gui/mycheckbox.h"

class TSelect : public ToolWithWidget
{
public:
    TSelect();

    void function(Action action, QMouseEvent *event);

signals:

public slots:

private:
    MyCheckBox *checkBox[2];
    QCheckBox *ignoreBackfacing;

    void countMinAndMax(QVector2D *min, QVector2D *max, const QVector2D &currentPosition);
    void _select(const QVector2D &min, const QVector2D &max);
};

#endif // TSELECT_H
