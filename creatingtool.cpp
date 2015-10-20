#include "creatingtool.h"

CreatingTool::CreatingTool(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{

}

void CreatingTool::removeAll(int triangleNumber)
{
    int i;
    for(i = 0; i < ver.size(); i++) model->getVertex()[ver[i]].remove();
    for(i = 0; i < tri.size(); i++) model->getTriangle()[tri[i]].remove();
}
