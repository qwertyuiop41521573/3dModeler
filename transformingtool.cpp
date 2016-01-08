#include "transformingtool.h"
#include "journal.h"
#include "target.h"

using namespace Target;

TransformingTool::TransformingTool() : ToolWithWidget()
{
    for(int i = 0; i < 3; i++)
    {
        spinBox[i] = new MySpinBox;
        layout->addWidget(spinBox[i], i + 1, 1);
    }
}

void TransformingTool::function(Action action, QMouseEvent *event)
{
    using namespace Model;
    if(action == START || action == FINAL)
    {
        _busy = true;
        Journal::newRecord(EDIT);

        toTransform.clear();
        int i;

        //fill list of selected vertices or vertices that belong to selected triangles
        if(workWithElements[0]->isChecked())
        {
            for(i = 0; i < vertex().size(); i++) if(vertex()[i].exists() && vertex()[i].selected()) toTransform.push_back(i);
        }
        else
        {
            int j;

            int index;
            //we nee "checked" no to repeat vertices that belong to more than 1 triangle
            checked.resize(vertex().size());
            for(i = 0; i < vertex().size(); i++) checked[i] = false;

            for(i = 0; i < triangle().size(); i++)
            {
                if(!triangle()[i].exists() || !triangle()[i].selected()) continue;

                for(j = 0; j < 3; j++)
                {
                    index = triangle()[i].getIndex(j);
                    if(checked[index]) continue;

                    checked[index] = true;
                    toTransform.push_back(index);
                }
            }
            checked.clear();
        }

        for(int i = 0; i < toTransform.size(); i++) Journal::addBefore(true, toTransform[i]);

        if(action == FINAL) action = EXECUTE;
    }
    if(action == STOP)
    {
        _busy = false;

        vector <TwoElementsWithIndex <Vertex> > &vertex = Journal::current()->data().edit->vertex();
        for(int i = 0; i < vertex.size(); i++) vertex[i].setAfter(Model::vertex()[vertex[i].index()]);

        Journal::submit();
    }
    //actual transformation
    if(action == EXECUTE)
    {
        for(int i = 0; i < toTransform.size(); i++) vertex()[toTransform[i]].setPosition(transformation * vertex()[toTransform[i]].position());
    }
}
