#include "toolwithpivot.h"
#include "glwidget.h"

ToolWithPivot::ToolWithPivot(MainWindow *mainWindow) : TransformingTool(mainWindow)
{

}

void ToolWithPivot::function(Action action, QMouseEvent *event)
{
    if(action != START && action != FINAL) return;

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->vertex();
    vector <Triangle> &triangle = model->triangle();
    int i, j, k;

    QVector3D min, max;
    if(workWithElements[0]->isChecked())
    {
        //find first vertex
        for(i = 0; i < vertex.size(); i++)
        {
            if(!vertex[i].exists() || !vertex[i].selected()) continue;

            min = max = vertex[i].positionRO();
            break;
        }
        //expand bounding box to contain all other selected vertices
        for(i++; i < vertex.size(); i++)
        {
            if(!vertex[i].exists() || !vertex[i].selected()) continue;

            for(j = 0; j < 3; j++)
            {
                if(vertex[i].positionRO()[j] > max[j]) max[j] = vertex[i].positionRO()[ j ];
                if(vertex[i].positionRO()[j] < min[j]) min[j] = vertex[i].positionRO()[j];
            }
        }
    }
    else
    {
        //similar with triangles
        int index;
        for(i = 0; i < triangle.size(); i++)
        {
            if(!triangle[i].exists() || !triangle[i].selected()) continue;

            min = max = vertex[triangle[i].getIndex(0)].positionRO();
            break;
        }

        checked.resize(vertex.size());
        for(i = 0; i < vertex.size(); i++) checked[i] = false;
        for( ; i < triangle.size(); i++)
        {
            if(!triangle[i].exists() || !triangle[i].selected()) continue;

            for(j = 0; j < 3; j++)
            {
                index = triangle[i].getIndex(j);
                if(checked[index]) continue;

                for(k = 0; k < 3; k++)
                {
                    checked[index] = true;
                    if(vertex[index].positionRO()[k] > max[k]) max[k] = vertex[index].positionRO()[k];
                    if(vertex[index].positionRO()[k] < min[k]) min[k] = vertex[index].positionRO()[k];
                }
            }
        }
        checked.clear();
    }
    //center of bounding box
    pivot = (min + max) / 2;

    //get on-viewport projection for this point
    widget->countFinalMatrix();
    widget->fromWorldToScreen(&pivotOnScreen, pivot);
}
