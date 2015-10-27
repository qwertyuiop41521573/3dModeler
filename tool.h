#ifndef TOOL_H
#define TOOL_H

#include <QPushButton>
#include <QVector3D>
#include <QtOpenGL>

class GLWidget;

//actions that tool can perform
//usually START   - mouse button pressed
//        EXECUTE - mouse moved (when pressed or when stage2 and released
//        STOP    - mouse button released, if tool has stage2 - pressed while stage2
//        STAGE2  - mouse button released (only if tool hasStage2)
//        DRAW    - transforming tools draw some guiding lines on viewport
//        FINAL   - finalButton was pressed (on tool's widget
typedef enum { START, STOP, EXECUTE, DRAW, FINAL, STAGE2 } Action;

class MainWindow;

class Tool : public QObject
{
    Q_OBJECT
public:
    Tool(MainWindow *mainWindow);

    virtual void setActive(bool value);
    //"function" contains actual job of tool
    virtual void function(Action action, QMouseEvent *event = 0) {};
    virtual bool stage2() {};

    QPushButton *getButton()
    { return button; };

    bool hasStage2()
    { return _hasStage2; };

    bool busy()
    { return _busy; };

    //some tools have options that can be used by pressing shift or ctrl
    QCheckBox *shift()
    { return _shift; };

    QCheckBox *ctrl()
    { return _ctrl; };

protected:
    MainWindow *_mainWindow;
    bool _hasStage2 = false;
    //pointer to pointer to active viewport, pointer to current active viewport is (*_activeWidget)
    //class GLWidget has the same pointer to active tool
    GLWidget **_activeWidget;
    //button to switch to this tool
    QPushButton *button;

    bool _busy = false;

    QCheckBox *_shift = 0;
    QCheckBox *_ctrl = 0;

private slots:
    void handleClick(bool pressed);

private:
    bool _isActive = false;


};

#endif // TOOL_H

