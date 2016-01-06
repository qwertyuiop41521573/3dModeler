#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.h"
#include "journal.h"

#include "gui/mypushbutton.h"
#include "gui/mycombobox.h"

#include "tools/tpan.h"
#include "tools/tzoom.h"
#include "tools/trotatecamera.h"
#include "tools/torbit.h"
#include "tools/tselect.h"
#include "tools/tmove.h"
#include "tools/tscale.h"
#include "tools/trotate.h"
#include "tools/tvertex.h"
#include "tools/ttriangle.h"
#include "tools/tplane.h"
#include "tools/tbox.h"
#include "tools/tellipse.h"
#include "tools/tcylinder.h"
#include "tools/tsphere.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    Tool **getActiveTool()
    { return &toolActive; }

    QRadioButton **getWorkWithElements()
    { return workWithElements; };

    void setActiveWidget(GLWidget *widget);

    GLWidget **getActiveWidget()
    { return &widgetActive; };

    Journal *getJournal()
    { return &journal; };

    void quickAccessToolOrbit();
    void quickAccessToolPan();
    void stopQuickAccess();

    void hideViewport(int index);
    void setActiveTool(Tool *tool);

private slots:
    void open();

    void newModel();

    bool save();
    void saveAs();

    void handleClose();

    void undo();
    void redo();
    void selectAll();
    void selectNone();
    void snapTogether();
    void weldTogether();
    void deleteSlot();

    void final()
    { toolActive->function(FINAL); };

    void changeRenderingMode(int mode)
    { widgetActive->setRenderingMode(RenderingMode(mode)); };

    void changeWireFrameOverlay()
    { widgetActive->setWireframeOverlay(!widgetActive->getWireframeOverlay()); };

    void changeProjection(int newProjection)
    { widgetActive->setProjection(Projection(newProjection)); };

    void maximize(bool value);

private:
    //pointers to other tools
    //toolActive is used like widgetActive
    Tool *toolActive, *lastTool;
    //viewports
    //there are only 4 vieports, widgetActive is a pointer to one of them
    GLWidget *widget[4], *widgetActive;

    MyComboBox *renderingMode;
    QCheckBox *wireframeOverlay;
    MyComboBox *projection;

    MyPushButton *hideViewportButtons[4];
    QPushButton *maximizeButton;

    //tools
    //without widget
    TPan *tPan;
    TZoom *tZoom;
    TRotateCamera *tRotateCamera;
    TOrbit *tOrbit;

    //with widget (displayed under all tool buttons)
    TSelect *tSelect;

    //transforming tools
    TMove *tMove;
    TScale *tScale;
    TRotate *tRotate;

    //creating tools
    TVertex *tVertex;
    TTriangle *tTriangle;
    TPlane *tPlane;
    TBox *tBox;
    TEllipse *tEllipse;
    TCylinder *tCylinder;
    TSphere *tSphere;

    //vertices or triangles
    QRadioButton *workWithElements[2];

    //for undo / redo
    Journal journal;

    void createActionsAndMenus();
    bool saveRequest();
    bool openFileDialog(QString title);
    void addToVertexList2(vector <int> *vertexList, vector <int> *vertexList2, int index);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
