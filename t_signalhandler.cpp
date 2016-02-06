#include "t_signalhandler.h"
#include "model.h"
#include "journal.h"
#include "target.h"
#include "trianglecontainer.h"

#include <QAction>

using namespace Target;

ToolSet::SignalHandler::SignalHandler(QAction *selectAllAction, QAction *selectNoneAction, QAction *snapTogetherAction, QAction *weldTogetherAction, QAction *deleteAction) : QObject(0)
{
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));
    connect(selectNoneAction, SIGNAL(triggered()), this, SLOT(selectNone()));
    connect(snapTogetherAction, SIGNAL(triggered()), this, SLOT(snapTogether()));
    connect(weldTogetherAction, SIGNAL(triggered()), this, SLOT(weldTogether()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSlot()));
}

void ToolSet::SignalHandler::selectAll()
{
    using namespace Model;
    Journal::newRecord(EDIT);
    //if we work with vertices
    if(Target::isVertex())
    {
        for(int i = 0; i < vertex().size(); i++) if(vertex()[i].exists()) vertex().setSelected(i, true);
    }
    //or triangles
    else
    {
        for(tr_it it = triangle().begin(); it != triangle().end(); it++)
            if(it->exists()) triangle().setSelected(it, true);
    }
    Journal::submit();
}

void ToolSet::SignalHandler::selectNone()
{
    using namespace Model;
    Journal::newRecord(EDIT);
    if(Target::isVertex())
    {
        for(int i = 0; i < vertex().size(); i++) if(vertex()[i].exists()) vertex().setSelected(i, false);
    }
    else
    {
        for(tr_it it = triangle().begin(); it != triangle().end(); it++)
            if(it->exists()) triangle().setSelected(it, false);
    }
    Journal::submit();
}

void ToolSet::SignalHandler::snapTogether()
{
    using namespace Model;
    //only for vertices
    if(Target::isTriangle()) return;

    vector <int> selected;
    QVector3D min, max;

    //find first vertex in list and set min and max to it's coordinates
    int i;
    for(i = 0; i < vertex().size(); i++)
    {
        if(!vertex()[i].exists() || !vertex()[i].selected()) continue;

        min = max = vertex()[i].position();
        selected.push_back(i);
        break;
    }
    //if no vertices selected
    if(i == vertex().size()) return;
    //expand bounding box with corners in min and max to fit all selected vertices
    for(i++; i < vertex().size(); i++)
    {
        if(!vertex()[i].exists() || !vertex()[i].selected()) continue;

        selected.push_back(i);
        const QVector3D &pos = vertex()[i].position();

        if(pos.x() > max.x()) max.setX(pos.x());
        if(pos.y() > max.y()) max.setY(pos.y());
        if(pos.z() > max.z()) max.setZ(pos.z());

        if(pos.x() < min.x()) min.setX(pos.x());
        if(pos.y() < min.y()) min.setY(pos.y());
        if(pos.z() < min.z()) min.setZ(pos.z());
    }
    //center of bounding box
    QVector3D center = (max + min) / 2;
    Journal::newRecord(EDIT);
    for(i = 0; i < selected.size(); i++)
    {
        Journal::addBefore(true, selected[i]);
        //move to center
        vertex()[selected[i]].setPosition(center);
        Journal::addAfter(true);
    }
}

void ToolSet::SignalHandler::weldTogether()
{
    using namespace Model;
    if(Target::isTriangle()) return;

    // GROUP is vector<int>, that contains indices of vertices, that have the same coordinates
    //we need 'vector<GROUP> groups' if there are more than 1 groups of vertices that were snapped together
    vector<vector<int> > groups;

    //spread all vertices to groups with same coordinates
    for(int i = 0; i < vertex().size(); i++)
    {
        if(!vertex()[i].exists() || !vertex()[i].selected()) continue;

        //search for previous vertex with same coordinates (in groups, not Model::vertex())
        int j;
        for(j = 0; j < groups.size(); j++)
        {
            if(vertex()[i].position() != vertex()[groups[j][0]].position()) continue;
            //if found, add current vertex to appropriate group
            groups[j].push_back(i);
            break;
        }
        //if found
        if(j < groups.size()) continue;

        //if this is first vertex with such coordinates, create GROUP with 1 element - this vertex
        groups.resize(groups.size() + 1);
        groups.back().push_back(i);
    }

    //search for first group that has more than 1 vertex
    //we don't need those that have 1 vertex - there's nothing to weld
    int i;
    for(i = 0; i < groups.size(); i++)
    {
        if(groups[i].size() == 1) continue;

        //if found, create journal record;
        Journal::newRecord(EDIT);
        break;
    }

    //if found, weld vertices in all such groups
    for( ; i < groups.size(); i++)
    {
        if(groups[i].size() == 1) continue;

        //loop through all vertices, except first one ( [0] )
        for(int j = 1; j < groups[i].size(); j++)
        {
            int index = groups[i][j];
            Journal::addBefore(true, index);
            //remove vertex
            vertex().remove(index);
            Journal::addAfter(true);
            //replace this vertex with first from group in all triangles
            for(tr_it it = triangle().begin(); it != triangle().end(); it++)
            {
                if(!it->exists()) continue;
                for(int l = 0; l < 3; l++)
                {
                    if(it->index(l) == index)
                    {
                        Journal::addBefore(it);
                        it->setIndex(l, groups[i][0]);
                        Journal::addAfter(false);
                        break;
                    }
                }
            }
        }
    }
}

void ToolSet::SignalHandler::deleteSlot()
{
    using namespace Model;
    vector <int> vertexList, vertexList2;
    VectUniqe<tr_it> triangleList;

    Journal::newRecord(DELETE);

    if(Target::isVertex()) {
        //add to vertexList all selected vertices, remove them
        for(int i = 0; i < vertex().size(); i++) {
            const Vertex &v = vertex()[i];
            if(!v.exists() || !v.selected()) continue;

            vertexList.push_back(i);
            vertex().remove(i);

            for(int j = 0; j < v.triangles().size(); j++) {
                const tr_it it = v.triangles()[j];
                if(triangleList.push(it)) {
                    for(int l = 0; l < 3; l++) if(it->index(l) != i)
                        addToVertexList2(&vertexList, &vertexList2, it->index(l));
                }
            }
        }
    }
    else
    {
        //add to triangleList all selected triangles
        for(tr_it it = triangle().begin(); it != triangle().end(); it++) {
            if(!it->exists() || !it->selected()) continue;

            triangleList.push_back(it);
            //and all vertices of this triangle
            for(int j = 0; j < 3; j++) addToVertexList2(&vertexList, &vertexList2, it->index(j));
        }
    }

    //remove triangles from list
    for(int i = 0; i < triangleList.size(); i++)
        triangle().rem(triangleList[i]);

    for(int i = 0; i < vertexList2.size(); i++) {
        //vertex()[vertexList2[i]].countNormals();
        if(vertex()[vertexList2[i]].triangles().empty())
            vertex().remove(vertexList2[i]);

    }

    Journal::submit();
}

void ToolSet::SignalHandler::addToVertexList2(vector <int> *vertexList, vector <int> *vertexList2, int index)
{
    //check if this vertex already is in vertexList 1 or 2
    for(int i = 0; i < vertexList->size(); i++)
        if((*vertexList)[i] == index) return;
    for(int i = 0; i < vertexList2->size(); i++)
        if((*vertexList2)[i] == index) return;
    vertexList2->push_back(index);
}
