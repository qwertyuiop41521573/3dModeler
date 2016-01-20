#include "toolwithpivot.h"
#include "glwidget.h"
#include "workspace.h"
#include "target.h"
#include "trianglecontainer.h"

using namespace Model;
using namespace Target;

ToolWithPivot::ToolWithPivot() : TransformingTool()
{

}

void ToolWithPivot::function(Action action, QMouseEvent *event)
{
    if(action != START && action != FINAL) return;

    GLWidget *widget = Workspace::activeWidget();
    int i, j, k;

    QVector3D min, max;
    if(workWithElements[0]->isChecked())
    {
        //find first vertex
        for(i = 0; i < vertex().size(); i++)
        {
            if(!vertex()[i].exists() || !vertex()[i].selected()) continue;

            min = max = vertex()[i].position();
            break;
        }
        //expand bounding box to contain all other selected vertices
        for(i++; i < vertex().size(); i++)
        {
            if(!vertex()[i].exists() || !vertex()[i].selected()) continue;

            for(j = 0; j < 3; j++)
            {
                if(vertex()[i].position()[j] > max[j]) max[j] = vertex()[i].position()[ j ];
                if(vertex()[i].position()[j] < min[j]) min[j] = vertex()[i].position()[j];
            }
        }
    }
    else
    {
        checked.resize(vertex().size());
        for(i = 0; i < vertex().size(); i++) checked[i] = false;

        //similar with triangles
        int index;
        tr_it it;
        for(it = triangle().begin(); it != triangle().end(); it++) {
            if(!it->exists() || !it->selected()) continue;

            min = max = it->vertex(0).position();
            break;
        }

        for( ; it != triangle().end(); it++)
        {
            if(!it->exists() || !it->selected()) continue;

            for(j = 0; j < 3; j++)
            {
                index = it->getIndex(j);
                if(checked[index]) continue;

                for(k = 0; k < 3; k++)
                {
                    checked[index] = true;
                    if(vertex()[index].position()[k] > max[k]) max[k] = vertex()[index].position()[k];
                    if(vertex()[index].position()[k] < min[k]) min[k] = vertex()[index].position()[k];
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
