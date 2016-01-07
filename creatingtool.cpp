#include "creatingtool.h"

CreatingTool::CreatingTool() : ToolWithWidget()
{

}

void CreatingTool::removeAll()
{
    int i;
    for(i = 0; i < ver.size(); i++) Model::vertex()[ver[i]].remove();
    for(i = 0; i < tri.size(); i++) Model::triangle()[tri[i]].remove();
}
