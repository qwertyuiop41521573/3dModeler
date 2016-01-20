#include "transformingtool.h"
#include "journal.h"
#include "target.h"
#include "trianglecontainer.h"

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
        if(action == START) _busy = true;
        Journal::newRecord(EDIT);

        toTransform.clear();

        //fill list of selected vertices or vertices that belong to selected triangles
        if(workWithElements[0]->isChecked())
        {
            for(int i = 0; i < vertex().size(); i++) if(vertex()[i].exists() && vertex()[i].selected()) toTransform.push_back(i);
        }
        else
        {
            int index;
            //we nee "checked" no to repeat vertices that belong to more than 1 triangle
            checked.resize(vertex().size());
            for(int i = 0; i < vertex().size(); i++) checked[i] = false;

            for(tr_it it = triangle().begin(); it != triangle().end(); it++) {
                if(!it->exists() || !it->selected())
                    continue;

                for(int j = 0; j < 3; j++)
                {
                    index = it->getIndex(j);
                    if(checked[index]) continue;

                    checked[index] = true;
                    toTransform.push_back(index);
                }
            }
            checked.clear();
        }

        tri.clear();
        for(int i = 0; i < toTransform.size(); i++) {
            for(tr_it it = triangle().begin(); it != triangle().end(); it++) {
                for(int k = 0; k < 3; k++) {
                    if(toTransform[i] == it->getIndex(k)) {
                        int l;
                        for(l = 0; l < tri.size(); l++)
                            if(tri[l] == it) break;
                        if(l == tri.size()) tri.push_back(it);
                        break;
                    }
                }
            }
        }


        for(int i = 0; i < toTransform.size(); i++) Journal::addBefore(true, toTransform[i]);

        if(action == FINAL) {
            TransformingTool::function(EXECUTE, 0);
            TransformingTool::function(STOP, 0);
        }
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
        updateNormals();
    }
}

void TransformingTool::updateNormals()
{
    for(int i = 0; i < tri.size(); i++) tri[i]->countNormal();
}
