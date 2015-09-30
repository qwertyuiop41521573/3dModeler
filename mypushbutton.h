#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

#include <iostream>

using namespace std;

class MainWindow;

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(int index, MainWindow *mainWindow,
                          QWidget *parent = 0);

public slots:
    void handleClick();

private:
    int _index;
    MainWindow *_mainWindow;
};

#endif // MYPUSHBUTTON_H
