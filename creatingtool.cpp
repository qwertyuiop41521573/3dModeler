#include "creatingtool.h"

CreatingTool::CreatingTool(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{

}

void CreatingTool::removeAll()
{
    int i;
    for(i = 0; i < ver.size(); i++) model->vertex()[ver[i]].remove();
    for(i = 0; i < tri.size(); i++) model->triangle()[tri[i]].remove();
}
