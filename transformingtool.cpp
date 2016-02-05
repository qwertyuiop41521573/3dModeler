#include "transformingtool.h"
#include "journal.h"
#include "target.h"
#include "trianglecontainer.h"

#include "records/redit.h"

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
        if(Target::isVertex())
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
                if(!it->exists() || !it->selected()) continue;

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
            const Vertex &v = vertex()[toTransform[i]];
            for(int j = 0; j < v.triangles().size(); j++)
                tri.push(v.triangles()[j]);
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
        static_cast<REdit*>(Journal::temporary())->setAfter();
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
    vector <int> ver2;
    for(int i = 0; i < tri.size(); i++) {
        tri[i]->countNormal();
        for(int j = 0; j < 3; j++) {
            int k;

            for(k = 0; k < toTransform.size(); k++)
                if(tri[i]->getIndex(j) == toTransform[k]) break;
            if(k < toTransform.size()) continue;

            for(k = 0; k < ver2.size(); k++)
                if(tri[i]->getIndex(j) == ver2[k]) break;
            if(k < ver2.size()) continue;

            ver2.push_back(tri[i]->getIndex(j));
        }
    }
    for(int i = 0; i < toTransform.size(); i++)
        Model::vertex()[toTransform[i]].countNormals();
    for(int i = 0; i < ver2.size(); i++) Model::vertex()[ver2[i]].countNormals();
}
