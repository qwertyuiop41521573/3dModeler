#include "toolwithpivot.h"
#include "glwidget.h"

ToolWithPivot::ToolWithPivot(MainWindow *mainWindow) : TransformingTool(mainWindow)
{

}

void ToolWithPivot::function(Action action, QMouseEvent *event)
{
    if(action != START && action != FINAL) return;

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int i, j, k;

    QVector3D min, max;
    if(workWithElements[0]->isChecked())
    {
        for(i = 0; i < vertexSize; i++)
        {
            if(vertex[i].selected())
            {
                min = max = vertex[i].getPosition();
                break;
            }
        }
        for(i++; i < vertexSize; i++)
        {
            if(vertex[i].selected())
            {
                for(j = 0; j < 3; j++)
                {
                    if(vertex[i].getPosition()[j] > max[j]) max[j] =
                                vertex[i].getPosition()[ j ];
                    if(vertex[i].getPosition()[j] < min[j]) min[j] =
                                vertex[i].getPosition()[j];
                }
            }
        }
    }
    else
    {
        int index;
        for(i = 0; i < triangleSize; i++)
        {
            if(triangle[i].selected())
            {
                min = max = vertex[triangle[i].getIndex(0)].getPosition();
                break;
            }
        }

        checked.resize(vertexSize);
        for(i = 0; i < vertexSize; i++) checked[i] = false;
        for( ; i < triangleSize; i++)
        {
            if(triangle[i].selected())
            {
                for(j = 0; j < 3; j++)
                {
                    index = triangle[i].getIndex(j);
                    if(!checked[index])
                    {
                        for(k = 0; k < 3; k++)
                        {
                            checked[index] = true;
                            if(vertex[index].getPosition()[k] > max[k])
                                max[k] = vertex[index].getPosition()[k];
                            if(vertex[index].getPosition()[k] < min[k])
                                min[k] = vertex[index].getPosition()[k];
                        }
                    }
                }
            }
        }
        checked.clear();
    }
    pivot = (min + max);
    for(i = 0; i < 3; i++) pivot[i] /= 2;
    widget->fromWorldToScreen(pivotOnScreen, pivot, true);
}
