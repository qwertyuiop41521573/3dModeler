#include "creatingtool.h"

CreatingTool::CreatingTool(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{

}

void CreatingTool::removeAll(int triangleNumber)
{
    for(int i = 0; i < ind.size(); i++) model->getVertex()[ind[i]].remove();
    vector <Triangle> &triangle = model->getTriangle();
    triangle.erase(triangle.end() - triangleNumber, triangle.end());
}
