#include "transformingtool.h"

TransformingTool::TransformingTool(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{

}

void TransformingTool::transform()
{
    vector <Vertex> &vertex = model->getVertex();
    int vertexSize = vertex.size();
    int i;

    if(workWithElements[0]->isChecked())
    {
        for(i = 0; i < vertexSize; i++) if(vertex[i].selected()) toTransform.push_back(i);
    }
    else
    {
        vector <Triangle> &triangle = model->getTriangle();
        int triangleSize = triangle.size();
        int j;

        int index;
        checked.resize(vertexSize);
        for(i = 0; i < vertexSize; i++) checked[i] = false;

        for(i = 0; i < triangleSize; i++)
        {
            if(triangle[i].selected())
            {
                for(j = 0; j < 3; j++)
                {
                    index = triangle[i].getIndex(j);
                    if(!checked[index])
                    {
                        checked[index] = true;
                        toTransform.push_back(index);
                    }
                }
            }
        }
        checked.clear();
    }
    for(i = 0; i < toTransform.size(); i++) vertex[toTransform[i]].setPosition(transformation * vertex[toTransform[i]].getPosition());
    toTransform.clear();
}

