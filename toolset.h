#ifndef TOOLSET_H
#define TOOLSET_H

class Tool;

class QGridLayout;

namespace ToolSet
{
    void init();
    void insertButtons(QGridLayout *layout);
    void insertWidgets(QGridLayout *layout);

    Tool *activeTool();
    void setActiveTool(Tool *tool);

    void quickAccessToolOrbit();
    void quickAccessToolPan();
    void stopQuickAccess();

    void hideCylinderWidget();
}

#endif // TOOLSET_H
