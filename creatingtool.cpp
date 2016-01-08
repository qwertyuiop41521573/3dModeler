#include "creatingtool.h"

CreatingTool::CreatingTool() : ToolWithWidget()
{

}

void CreatingTool::removeAll()
{
    for(int i = 0; i < ver.size(); i++) Model::vertex()[ver[i]].remove();
    for(int i = 0; i < tri.size(); i++) Model::triangle()[tri[i]].remove();
}
