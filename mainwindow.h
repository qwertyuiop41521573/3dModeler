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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    Model *getModel();

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

    void newFile()
    { if(saveRequest()) model->clear(); };

    void save();
    void saveAs()
    { if(openFileDialog("Save") ) model->save(); };

    void undo();
    void redo();
    void selectAll();
    void selectNone();
    void snapTogether();
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
    Model *model;
    Tool *toolActive, *lastTool;
    GLWidget *widget[4], *widgetActive;

    MyComboBox *renderingMode;
    QCheckBox *wireframeOverlay;
    MyComboBox *projection;

    MyPushButton *hideViewportButtons[4];
    QPushButton *maximizeButton;

    TPan *tPan;
    TZoom *tZoom;
    TRotateCamera *tRotateCamera;
    TOrbit *tOrbit;

    TSelect *tSelect;
    TMove *tMove;
    TScale *tScale;
    TRotate *tRotate;
    TVertex *tVertex;
    TTriangle *tTriangle;
    TPlane *tPlane;
    TBox *tBox;
    TEllipse *tEllipse;
    TCylinder *tCylinder;

    QRadioButton *workWithElements[2];

    Journal journal;

    void createActionsAndMenus();
    bool saveRequest();
    bool openFileDialog(QString title);
    void addToVertexList2(vector <int> *vertexList, vector <int> *vertexList2, int index);
};

#endif // MAINWINDOW_H
