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
#include "mycombobox.h"


#include <iostream>

#include "tpan.h"
#include "tzoom.h"
#include "trotatecamera.h"
#include "torbit.h"
#include "tselect.h"

#include "toolwithwidget.h"

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    Model *getModel();
    Tool **getActiveTool()
    { return &toolActive; }
    WidgetElements *getWorkWithElements();
    void setActiveWidget(GLWidget *widget);
    GLWidget **getActiveWidget()
    { return &widgetActive; };

    void quickAccessToolOrbit();
    void quickAccessToolPan();
    void stopQuickAccess();

    void hideViewport(int index);
    void setActiveTool(Tool *tool);


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

    void final()
    { toolActive->function(FINAL, 0, 0 ); };

    void changeRenderingMode(int mode)
    { widgetActive->setRenderingMode(RenderingMode(mode)); };

    void changeWireFrameOverlay()
    { widgetActive->setWireframeOverlay(!widgetActive->
                                        getWireframeOverlay()); };

    void changeProjection( int newProjection )
    { widgetActive->setProjection(
                    Projection( newProjection ) ); };






    void handleSelectClick( MyCheckBox *myCheckBox );

    void maximize(bool value);

private:
    QWidget *centralWidget;
    Model *model;

    Tool *toolActive, *lastTool;
    GLWidget *widget[4], *widgetActive;

    MyComboBox *renderingMode;
    QCheckBox *wireframeOverlay;
    MyComboBox *projection;
    WidgetElements *workWithElements;



    QMenu *fileMenu, *editMenu;
    QAction *openAction;
    QAction *exitAction;
    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;

    QAction *selectAllAction, *selectNoneAction, *snapTogetherAction;
    QAction *deleteAction;
    MyPushButton *hideViewportButtons[4];
    QPushButton *maximizeButton;
    QCursor windowCursor;

    void createActions();
    void createMenus();



    bool saveRequest();
    bool openFileDialog( QString title );

    ToolWithWidget *toolSelect, *toolMove, *toolScale, *toolRotate;
    ToolWithWidget *toolVertex, *toolPlane, *toolTriangle, *toolBox, *toolEllipse, *toolCylinder;


    TPan *tPan;
    TZoom *tZoom;
    TRotateCamera *tRotateCamera;
    TOrbit *tOrbit;
    TSelect *tSelect;

};

#endif // MAINWINDOW_H
