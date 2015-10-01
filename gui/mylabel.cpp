#include "mylabel.h"

MyLabel::MyLabel(QString text, int maxWidth) : QLabel(text)
{
    setMaximumWidth(maxWidth);
    setAlignment(Qt::AlignRight);
}

