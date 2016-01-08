#include "toolset.h"
#include "t_signalhandler.h"

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

#include <QGridLayout>

namespace ToolSet
{
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

    Tool *_activeTool, *lastTool;


    SignalHandler *signalHandler;
}

void ToolSet::init()
{
    tPan = new TPan();
    tZoom = new TZoom();
    tRotateCamera = new TRotateCamera();
    tOrbit = new TOrbit();

    tSelect = new TSelect();
    tMove = new TMove();
    tScale = new TScale();
    tRotate = new TRotate();
    tVertex = new TVertex();
    tTriangle = new TTriangle();
    tPlane = new TPlane();
    tBox = new TBox();
    tEllipse = new TEllipse();
    tCylinder = new TCylinder();
    tSphere = new TSphere();

    _activeTool = tSelect;
    setActiveTool(tSelect);
}

void ToolSet::insertButtons(QGridLayout *layout)
{
    QGridLayout *l = layout;
    l->addWidget(tPan->getButton(), 9, 0, 1, 2);
    l->addWidget(tZoom->getButton(), 9, 2, 1, 2);
    l->addWidget(tRotateCamera->getButton(), 10, 0, 1, 2);
    l->addWidget(tOrbit->getButton(), 10, 2, 1, 2);
    l->addWidget(tSelect->getButton(), 12, 0, 1, 2);
    l->addWidget(tMove->getButton(), 12, 2, 1, 2);
    l->addWidget(tScale->getButton(), 13, 0, 1, 2);
    l->addWidget(tRotate->getButton(), 13, 2, 1, 2);
    l->addWidget(tVertex->getButton(), 15, 0, 1, 2);
    l->addWidget(tTriangle->getButton(), 15, 2, 1, 2);
    l->addWidget(tPlane->getButton(), 16, 0, 1, 2);
    l->addWidget(tBox->getButton(), 16, 2, 1, 2);
    l->addWidget(tEllipse->getButton(), 17, 0, 1, 2);
    l->addWidget(tCylinder->getButton(), 17, 2, 1, 2);
    l->addWidget(tSphere->getButton(), 18, 0, 1, 2);
}

void ToolSet::insertWidgets(QGridLayout *layout)
{
    QGridLayout *l = layout;
    l->addWidget(tSelect->getWidget(), 22, 0, 1, 4);
    l->addWidget(tMove->getWidget(), 23, 0, 1, 4);
    l->addWidget(tScale->getWidget(), 24, 0, 1, 4);
    l->addWidget(tRotate->getWidget(), 25, 0, 1, 4);
    l->addWidget(tVertex->getWidget(), 26, 0, 1, 4);
    l->addWidget(tTriangle->getWidget(), 27, 0, 1, 4);
    l->addWidget(tPlane->getWidget(), 28, 0, 1, 4);
    l->addWidget(tBox->getWidget(), 29, 0, 1, 4);
    l->addWidget(tEllipse->getWidget(), 30, 0, 1, 4);
    l->addWidget(tCylinder->getWidget(), 31, 0, 1, 4);
    l->addWidget(tSphere->getWidget(), 32, 0, 1, 4);
}

Tool *ToolSet::activeTool() { return _activeTool; }

void ToolSet::setActiveTool(Tool *tool)
{
    if(_activeTool->hasStage2() && _activeTool->stage2())
    {
        tool->getButton()->setChecked(false);
        return;
    }
    //deactivate current
    _activeTool->setActive(false);
    //copy pointer to toolActive
    _activeTool = tool;
    //activate new
    tool->setActive(true);
}

void ToolSet::quickAccessToolOrbit()
{
    lastTool = _activeTool;
    setActiveTool(tOrbit);
}

void ToolSet::quickAccessToolPan()
{
    lastTool = _activeTool;
    setActiveTool(tPan);
}

void ToolSet::stopQuickAccess() { setActiveTool(lastTool); }

void ToolSet::hideCylinderWidget()
{
    //tCylinder->widget() was not hidden in it's constructor as it is the biggest widget
    tCylinder->getWidget()->hide();
}

void ToolSet::connectActions(QAction *selectAllAction, QAction *selectNoneAction, QAction *snapTogetherAction, QAction *weldTogetherAction, QAction *deleteAction)
{
    signalHandler = new SignalHandler(selectAllAction, selectNoneAction, snapTogetherAction, weldTogetherAction, deleteAction);
}
