#include "workspace.h"
#include "glwidget.h"
#include "w_signalhandler.h"

#include "gui/mypushbutton.h"
#include "gui/mycombobox.h"

#include <QObject>


namespace Workspace
{
    MyComboBox *renderingMode;
    QCheckBox *wireframeOverlay;
    MyComboBox *projection;

    //viewports
    //there are only 4 vieports, widgetActive is a pointer to one of them
    GLWidget *widget[4], *_activeWidget;

    MyPushButton *hideViewportButtons[4];
    QPushButton *maximizeButton;


    void initWidgets();
    void createRenderingMode();
    void createProjection();
    void initButtons();


    SignalHandler *signalHandler;
}

void Workspace::init()
{
    createRenderingMode();
    wireframeOverlay = new QCheckBox("Wireframe Overlay");
    createProjection();
    initWidgets();
    initButtons();
    //SignalHandler::create();
    signalHandler = new SignalHandler(maximizeButton, renderingMode, wireframeOverlay, projection);
}

void Workspace::initWidgets()
{
    for(int i = 0; i < 4; i++) widget[i] = new GLWidget();

    widget[0]->setProjection(TOP);
    widget[1]->setProjection(FRONT);
    widget[2]->setProjection(LEFT);
    widget[3]->setProjection(PERSPECTIVE);

    _activeWidget = widget[3];
    widget[3]->setRenderingMode(SMOOTH_SHADED);
    widget[3]->setWireframeOverlay(true);
    setActiveWidget(widget[3]);
}

void Workspace::createRenderingMode()
{
    renderingMode = new MyComboBox;

    renderingMode->addItem("Wireframe");
    renderingMode->addItem("Flat Shaded");
    renderingMode->addItem("Smooth Shaded");
    renderingMode->addItem("Textured");
}

void Workspace::createProjection()
{
    projection = new MyComboBox;

    projection->addItem("Top");
    projection->addItem("Bottom");
    projection->addItem("Front");
    projection->addItem("Back");
    projection->addItem("Left");
    projection->addItem("Right");
    projection->addItem("Perpective");
}

void Workspace::initButtons()
{
    for(int i = 0; i < 4; i++) hideViewportButtons[i] = new MyPushButton(i);

    maximizeButton = new QPushButton("Maximize\nActive");
    maximizeButton->setMaximumWidth(75);
    maximizeButton->setCheckable(true);
}

void Workspace::insertViewports(QGridLayout *layout)
{ for(int i = 0; i < 4; i++) layout->addWidget(widget[i], i / 2, i % 2); }

void Workspace::insertElements(QGridLayout *layout)
{
    QGridLayout *l = layout;

    l->addWidget(new QLabel("Rendering Mode:"), 0, 0, 1, 4);
    l->addWidget(renderingMode, 1, 0, 1, 4);
    l->addWidget(wireframeOverlay, 2, 0, 1, 4);
    l->addWidget(new QLabel("Projection:"), 3, 0, 1, 4);
    l->addWidget(projection, 4, 0, 1, 4);
    l->addWidget(new QLabel("Viewports:"), 5, 0, 1, 4);

    for(int i = 0; i < 4; i++)
        l->addWidget(hideViewportButtons[i], 6 + i / 2, i % 2);
    l->addWidget(maximizeButton, 6, 2, 2, 2);
}

void Workspace::setActiveWidget(GLWidget *widget)
{
    _activeWidget->setActive(false);
    _activeWidget = widget;
    widget->setActive(true);

    //load parameters from new active widget
    renderingMode->setCurrentIndex(widget->getRenderingMode());
    wireframeOverlay->setChecked(widget->getWireframeOverlay());
    projection->setCurrentIndex(widget->getProjection());
}

GLWidget *Workspace::activeWidget() { return _activeWidget; };

void Workspace::hideViewport(int index)
{
    widget[index]->setHidden(!widget[index]->isHidden());
    //if three viewports are hidden, it's equal to maximizing one viewport
    bool threeViewportsHidden = true;
    for(int i = 0; i < 4; i++) if(widget[i] != _activeWidget) threeViewportsHidden *= hideViewportButtons[i]->isChecked();
    maximizeButton->setChecked(threeViewportsHidden && !_activeWidget->isHidden());
}

void Workspace::SignalHandler::maximize(bool value)
{
    if(value)
    {
        for(int i = 0; i < 4; i++)
        {
            widget[i]->setOldHidden(widget[i]->isHidden());
            hideViewportButtons[i]->setChecked(!(widget[i] == _activeWidget));
        }
    }
    else for(int i = 0; i < 4; i++) hideViewportButtons[i]->setChecked(widget[i]->oldHidden());
}

void Workspace::SignalHandler::changeRenderingMode(int mode)
{ _activeWidget->setRenderingMode(RenderingMode(mode)); }

void Workspace::SignalHandler::changeWireFrameOverlay()
{ _activeWidget->setWireframeOverlay(!_activeWidget->getWireframeOverlay()); }

void Workspace::SignalHandler::changeProjection(int newProjection)
{ _activeWidget->setProjection(Projection(newProjection)); }


