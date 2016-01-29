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
    if(workWithElements[0]->isChecked())
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
    if(workWithElements[0]->isChecked())
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
    if(workWithElements[1]->isChecked()) return;

    int i;
    vector <int> selected;
    QVector3D min, max;

    //find first vertex in list and set min and max to it's coordinates
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
    if(workWithElements[1]->isChecked()) return;

    int i, j, k, l;

    // GROUP is vector <int>, that contains indices of vertices, that have the same coordinates
    //we need vector <GROUP> groups if there are more than 1 groups of vertices that were snapped together
    vector <vector <int> > groups;

    //spread all vertices to groups with same coordinates
    for(i = 0; i < vertex().size(); i++)
    {
        if(!vertex()[i].exists() || !vertex()[i].selected()) continue;

        //search for previous vertex with same coordinates (in groups, not vertex)
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
        groups[groups.size() - 1].push_back(i);
    }

    //search for first group that has more than 1 vertex
    //we don't need those that have 1 vertex - there's nothing to weld
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
        for(j = 1; j < groups[i].size(); j++)
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
                for(l = 0; l < 3; l++)
                {
                    if(it->getIndex(l) == index)
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
    int i, j, k;
    vector <int> vertexList, vertexList2;
    vector<tr_it> triangleList;

    Journal::newRecord(DELETE);

    if(workWithElements[0]->isChecked()) {
        //add to vertexList all selected vertices, remove them
        for(i = 0; i < vertex().size(); i++) {
            if(!vertex()[i].exists() || !vertex()[i].selected()) continue;

            vertexList.push_back(i);
            vertex().remove(i);
        }

        bool selected;
        int l;
        //loop though all triangles
        for(tr_it it = triangle().begin(); it != triangle().end(); it++) {
            if(!it->exists()) continue;
            selected = false;
            for(j = 0; j < 3; j++) {
                //check if this triangle's vertices were removed
                for(k = 0; k < vertexList.size(); k++)
                {
                    if(vertexList[k] != it->getIndex(j)) continue;
                    //if there is one vertex removed

                    //add this triangle to list
                    triangleList.push_back(it);
                    selected = true;
                    //and other 2 vertices to vertexList2
                    for(l = 0; l < 3; l++) if(l != j) addToVertexList2(&vertexList, &vertexList2, it->getIndex(l));
                    break;
                }
                if(selected) break;
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
            for(j = 0; j < 3; j++) addToVertexList2(&vertexList, &vertexList2, it->getIndex(j));
        }

    }

    //remove triangles from list
    for(i = 0; i < triangleList.size(); i++)
        triangle().rem(triangleList[i]);

    //check if there are some existing triangles (when deleting, they are just marked as not existing)
    for(tr_it it = triangle().begin(); it != triangle().end(); it++)
        if(it->exists()) break;
    bool noTriangles = i == triangle().size();

    for(i = 0; i < vertexList2.size(); i++) {
        //vertex should be removed if it does not belong to any triangle
        //if there are no triangles
        if(noTriangles) {
            vertex().remove(vertexList2[i]);
            continue;
        }
        //if there are some, we should check if one of them is built on this vertex
        tr_it it;
        for(it = triangle().begin(); it != triangle().end(); it++) {
            if(!it->exists()) continue;
            for(k = 0; k < 3; k++) if(it->getIndex(k) == vertexList2[i])
                break;
            if(k < 3) break;
        }
        //if no such triangle found, remove vertex
        if(it == triangle().end()) vertex().remove(vertexList2[i]);
    }

    Journal::submit();
}

void ToolSet::SignalHandler::addToVertexList2(vector <int> *vertexList, vector <int> *vertexList2, int index)
{
    //check if this vertex already is in vertexList 1 or 2
    for(int i = 0; i < vertexList->size(); i++) if((*vertexList)[i] == index) return;
    for(int i = 0; i < vertexList2->size(); i++) if((*vertexList2)[i] == index) return;
    vertexList2->push_back(index);
}
