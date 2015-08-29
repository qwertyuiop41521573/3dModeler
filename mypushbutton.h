#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

#include <iostream>

using namespace std;

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton( int newIndex, QWidget *parent = 0 );

signals:
    void handledClick( int emittedIndex );

public slots:
    void handleClick()
    { emit handledClick( index ); };

private:
    int index;
};

#endif // MYPUSHBUTTON_H
