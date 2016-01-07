#ifndef WORKSPACE_H
#define WORKSPACE_H

class GLWidget;
class QGridLayout;

namespace Workspace
{
    void init();

    void insertViewports(QGridLayout *layout);
    void insertElements(QGridLayout *layout);

    void setActiveWidget(GLWidget *widget);
    GLWidget *activeWidget();

    void hideViewport(int index);
}

#endif // WORKSPACE_H
