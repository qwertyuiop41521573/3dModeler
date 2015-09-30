#ifndef TOOL_H
#define TOOL_H

#include <QPushButton>
#include <QLineEdit>
#include <QVector3D>
#include <QtOpenGL>

//#include "model.h"

#include "widgetelements.h"

#include <iostream>

using namespace  std;

//class QString;
class GLWidget;
class QPoint;

void setupButton( QPushButton *button );

class VertexData_Color
{
public:
    VertexData_Color( QVector3D pos = { 0, 0, 0 },
                      QVector3D col = { 0, 0, 0 } )
    { position = pos; color = col; };

    QVector3D position;
    QVector3D color;
};

struct VertexAndIndexData
{
    vector <VertexData_Color> vertices;
    vector <GLushort> indices;
};

typedef enum { START, STOP, EXECUTE, DRAW, FINAL, STAGE2 } Action;

typedef void ( *toolFunction )( GLWidget *widget, Action action,
                  QMouseEvent *event, VertexAndIndexData *data );

class MainWindow;

class Tool : public QObject
{
    Q_OBJECT
public:
    Tool(MainWindow *mainWindow);

    virtual void setActive(bool value);
    virtual void function(Action action, QMouseEvent *event,
                          VertexAndIndexData *data) {};
    virtual bool stage2() {};

    QPushButton *getButton()
    { return button; };

    bool hasStage2()
    { return _hasStage2; };

protected:
    MainWindow *_mainWindow;
    bool _hasStage2 = false;
    GLWidget **_activeWidget;
    QPushButton *button;

private slots:
    void handleClick(bool pressed);

private:


    bool _isActive = false;
};

#endif // TOOL_H

