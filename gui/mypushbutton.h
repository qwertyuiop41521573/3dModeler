#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MainWindow;

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(int index);

public slots:
    void handleClick();

private:
    int _index;
};

#endif // MYPUSHBUTTON_H
