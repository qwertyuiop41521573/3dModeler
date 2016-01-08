#ifndef TOOLSET_H
#define TOOLSET_H

class Tool;

class QGridLayout;
class QAction;

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

    void connectActions(QAction *selectAllAction, QAction *selectNoneAction, QAction *snapTogetherAction, QAction *weldTogetherAction, QAction *deleteAction);
}

#endif // TOOLSET_H
