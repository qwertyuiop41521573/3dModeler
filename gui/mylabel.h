#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
public:
    MyLabel(const QString &text, int maxWidth);

signals:

public slots:
};

#endif // MYLABEL_H
