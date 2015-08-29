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

class Tool : public QObject
{
    Q_OBJECT
public:
    Tool( QString buttonText, toolFunction newFunction, Qt::
          CursorShape cursor, QWidget *widget = 0, WidgetElements
          *newElements = 0, QString finalButtonText = 0, bool hasStage2 = false );

    void setActive( bool value );
    void ( *function )( GLWidget *widget, Action action, QMouseEvent
                        *event, VertexAndIndexData *data );

    QPushButton *getButton()
    { return button; };

    QWidget *getWidget()
    { return _widget; };

    bool elementsExist();

    WidgetElements *getElements()
    { return elements; }

    QCursor cursor()
    { return _cursor; };

    QPushButton *getFinalButton()
    { return finalButton; };

    bool hasStage2()
    { return _hasStage2; };

    void setStage2( bool value )
    { _stage2 = value; };

    bool stage2()
    { return _stage2; };

signals:
    void makeMeActive( Tool *tool );
    void changeCursor( bool closedCursor );

private slots:
    void handleClick( bool pressed );

private:
    QPushButton *button;
    QPushButton *finalButton;
    bool _isActive = false;
    QWidget *_widget;
    WidgetElements *elements;
    QCursor _cursor;
    bool _hasStage2;
    bool _stage2 = false;
};

#endif // TOOL_H

