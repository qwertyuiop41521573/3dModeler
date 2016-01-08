#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
public:
    MyLabel(const QString &text, int maxWidth);
};

#endif // MYLABEL_H
