#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>

#include "glwidget.h"
#include "model.h"
#include "tool.h"
#include "functions.h"
#include "mypushbutton.h"

#include <iostream>

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void open();

    void newFile()
    { if( saveRequest() ) model->clear(); };

    void save();
    void saveAs()
    { if( openFileDialog( "Save" ) ) model->save(); };

    void selectAll();
    void selectNone();
    void snapTogether();
    void deleteSlot();

    void setActiveWidgetSlot( GLWidget *widget )
    { setActiveWidget( widget ); };

    void setActiveToolSlot( Tool *tool )
    { setActiveTool( tool ); };

    void final()
    { toolActive->function( widgetActive, FINAL, 0, 0 ); };

    void changeRenderingMode( int mode )
    { widgetActive->setRenderingMode(
                    RenderingMode( mode ) ); };

    void changeWireFrameOverlay()
    { widgetActive->setWireframeOverlay(
        !widgetActive->getWireframeOverlay() ); };

    void changeProjection( int newProjection )
    { widgetActive->setProjection(
                    Projection( newProjection ) ); };

    void quickAccessToolOrbit();
    void quickAccessToolPan();
    void quickAccessToolZoom();

    void stopQuickAccess()
    { setActiveTool( lastTool ); };



    void handleSelectClick( MyCheckBox *myCheckBox );
    void hideViewport( int index );
    void maximize( bool value );
    void changeCursor( bool cursorClosed );

private:
    QWidget *centralWidget;
    Model *model;
    Tool *toolPan, *toolZoom, *toolRotateCamera, *toolOrbit;
    Tool *toolActive, *lastTool;
    GLWidget *widget[ 4 ];
    GLWidget *widgetActive;

    QComboBox *renderingMode;
    QCheckBox *wireframeOverlay;
    QComboBox *projection;
    WidgetElements *workWithElements;

    Tool *toolSelect, *toolMove, *toolScale, *toolRotate;
    Tool *toolVertex, *toolPlane, *toolTriangle, *toolBox, *toolEllipse, *toolCylinder;

    QMenu *fileMenu, *editMenu;
    QAction *openAction;
    QAction *exitAction;
    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;

    QAction *selectAllAction, *selectNoneAction, *snapTogetherAction;
    QAction *deleteAction;
    MyPushButton *hideViewportButtons[ 4 ];
    QPushButton *maximizeButton;
    QCursor windowCursor;
    QTextBrowser *browser;

    void createActions();
    void createMenus();

    void setActiveWidget( GLWidget *widget );
    void setActiveTool( Tool *tool );
    bool saveRequest();
    bool openFileDialog( QString title );
};

#endif // MAINWINDOW_H
