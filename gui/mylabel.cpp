#include "mylabel.h"

MyLabel::MyLabel(const QString &text, int maxWidth) : QLabel(text)
{
    setMaximumWidth(maxWidth);
    setAlignment(Qt::AlignRight);
}

