#include "mainwindow.h"
#include "journal.h"
#include "target.h"

#include "gui/myframe.h"

#include <iostream>

using namespace std;
using namespace ToolSet;
using namespace Target;

MainWindow::MainWindow()
{
    Model::init();
    ToolSet::init();
    Workspace::init();
    initMainWindow();
}

void MainWindow::initMainWindow()
{
    createCentralWidget();
    createActionsAndMenus();
    setWindowTitle("3d Modeler");
}

void MainWindow::createCentralWidget()
{
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(createCentralLayout());
    setCentralWidget(centralWidget);
}

QGridLayout *MainWindow::createCentralLayout()
{
    QGridLayout *l = new QGridLayout;
    l->addWidget(createScrollArea(), 0, 1);
    l->addWidget(createViewportsWidget(), 0, 0);
    return l;
}

QScrollArea *MainWindow::createScrollArea()
{
    QScrollArea *s = new QScrollArea;
    s->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    s->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    s->setFixedWidth(225);
    s->setWidget(createScrollAreaWidget());
    ToolSet::hideCylinderWidget();
    return s;
}

QWidget *MainWindow::createScrollAreaWidget()
{
    QWidget *w = new QWidget;
    w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    createWorkWithWidget();
    w->setLayout(createScrollAreaLayout());
    return w;
}

void MainWindow::createWorkWithWidget()
{
    workWithWidget = new QWidget;
    QGridLayout *workWithLayout = new QGridLayout;
    for(int i = 0; i < 2; i++) workWithElements[i] = new QRadioButton;
    workWithElements[0]->setText("Vertex");
    workWithElements[1]->setText("Triangle");

    for(int i = 0; i < 2; i++) workWithLayout->addWidget(workWithElements[i], i, 0);

    workWithElements[0]->setChecked(true);
    workWithWidget->setLayout(workWithLayout);
}

QGridLayout *MainWindow::createScrollAreaLayout()
{
    QGridLayout *l = new QGridLayout;

    Workspace::insertElements(l);
    ToolSet::insertButtons(l);
    l->addWidget(workWithWidget, 20, 0, 1, 4);
    ToolSet::insertWidgets(l);

    l->addWidget(new MyFrame, 8, 0, 1, 4);
    l->addWidget(new MyFrame, 11, 0, 1, 4);
    l->addWidget(new MyFrame, 14, 0, 1, 4);
    l->addWidget(new MyFrame, 19, 0, 1, 4);
    l->addWidget(new MyFrame, 21, 0, 1, 4);

    l->addItem(new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding), 40, 0, 1, 4);

    return l;
}

QWidget *MainWindow::createViewportsWidget()
{
    QWidget *v = new QWidget;
    v->setLayout(createViewportsLayout());
    return v;
}

QGridLayout *MainWindow::createViewportsLayout()
{
    QGridLayout *l = new QGridLayout;
    Workspace::insertViewports(l);
    return l;
}

void MainWindow::open()
{
    using namespace Model;
    if(modified())
    {
         if(!saveRequest()) return;
    }
    if(openFileDialog("Open")) return;

    if(!empty()) clear();
    if(load(fileName().toStdString().c_str())) setWindowTitle("3d Modeler - " + fileName());
}

bool MainWindow::saveRequest()
{
    if(Model::empty()) return true;

    QMessageBox saveRequestMessage;
    saveRequestMessage.setText("The model has been modified.");
    saveRequestMessage.setInformativeText("Do you want to save your changes?");
    saveRequestMessage.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    saveRequestMessage.setDefaultButton(QMessageBox::Save);
    int result = saveRequestMessage.exec();
    switch(result)
    {
    case QMessageBox::Save:
    {
        return save();
    }
    case QMessageBox::Discard:
        return true;
    }
    return false;
}

bool MainWindow::save()
{
    using namespace Model;
    if(loaded()) return save();
    if(openFileDialog("Save")) return false;
    if(!save()) return false;

    setWindowTitle("3d Modeler - " + fileName());
    return true;
}

void MainWindow::saveAs()
{
    if(!openFileDialog("Save")) Model::save();
    setWindowTitle("3d Modeler - " + Model::fileName());
}

bool MainWindow::openFileDialog(QString action)
{
    QFileDialog dialog;
    QStringList filters;
    filters << "3dModeller (*.mdl)" << "All Files (*)";
    dialog.setNameFilters(filters);
    dialog.setWindowTitle(action + " Model");
    dialog.setLabelText(QFileDialog::Accept, action);
    if(!dialog.exec()) return false;

    dialog.selectedFiles();
    Model::setFileName(dialog.selectedFiles().join(""));
}

void MainWindow::newModel()
{
    if(Model::modified() ? saveRequest() : true) Model::clear();
    setWindowTitle("3d Modeler");
}

void MainWindow::handleClose()
{
    if(Model::modified() ? saveRequest() : true) close();
}

void MainWindow::createActionsAndMenus()
{
    createActions();
    createMenus();
}

void MainWindow::createActions()
{
    fileActions();
    editActions();
}

void MainWindow::fileActions()
{
    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(tr("Ctrl+O"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(handleClose()));
    newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newModel()));
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    saveAsAction = new QAction(tr("&Save As"), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
}

void MainWindow::editActions()
{
    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcut(tr("Ctrl+Shift+Z"));
    Journal::connectActions(undoAction, redoAction);

    selectAllAction = new QAction(tr("&Select All"), this);
    selectAllAction->setShortcut(tr("Ctrl+A"));
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));
    selectNoneAction = new QAction(tr("&Select None"), this);
    selectNoneAction->setShortcut(tr("Ctrl+D"));
    connect(selectNoneAction, SIGNAL(triggered()), this, SLOT(selectNone()));
    snapTogetherAction = new QAction(tr("&Snap Together"), this);
    snapTogetherAction->setShortcut(tr("Ctrl+T"));
    connect(snapTogetherAction, SIGNAL(triggered()), this, SLOT(snapTogether()));
    weldTogetherAction = new QAction(tr("&Weld Together"), this);
    weldTogetherAction->setShortcut(tr("Ctrl+W"));
    connect(weldTogetherAction, SIGNAL(triggered()), this, SLOT(weldTogether()));
    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSlot()));
}

void MainWindow::createMenus()
{
    fileMenu();
    editMenu();
}

void MainWindow::fileMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(exitAction);
}

void MainWindow::editMenu()
{
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addAction(selectAllAction);
    editMenu->addAction(selectNoneAction);
    editMenu->addAction(snapTogetherAction);
    editMenu->addAction(weldTogetherAction);
    editMenu->addAction(deleteAction);
}

void MainWindow::selectAll()
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
        for(int i = 0; i < triangle().size(); i++) if(triangle()[i].exists()) triangle().setSelected(i, true);
    }
    Journal::submit();
}

void MainWindow::selectNone()
{
    using namespace Model;
    Journal::newRecord(EDIT);
    if(workWithElements[0]->isChecked())
    {
        for(int i = 0; i < vertex().size(); i++) if(vertex()[i].exists()) vertex().setSelected(i, false);
    }
    else
    {
        for(int i = 0; i < triangle().size(); i++) if(triangle()[i].exists()) triangle().setSelected(i, false);
    }   
    Journal::submit();
}

void MainWindow::snapTogether()
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

        min = max = vertex()[i].positionRO();
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
        const QVector3D &pos = vertex()[i].positionRO();

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

void MainWindow::weldTogether()
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
            if(vertex()[i].positionRO() != vertex()[groups[j][0]].positionRO()) continue;
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
            for(k = 0; k < triangle().size(); k++)
            {
                if(!triangle()[k].exists()) continue;

                for(l = 0; l < 3; l++)
                {
                    if(triangle()[k].getIndex(l) == index)
                    {
                        Journal::addBefore(false, k);
                        triangle()[k].setIndex(l, groups[i][0]);
                        Journal::addAfter(false);
                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::deleteSlot()
{
    using namespace Model;
    int i, j, k;
    vector <int> vertexList, triangleList, vertexList2;

    Journal::newRecord(EDIT);

    bool chain;
    int end;
    if(workWithElements[0]->isChecked())
    {
        //add to vertexList all selected vertices, remove them
        for(i = 0; i < vertex().size(); i++)
        {
            if(!vertex()[i].exists() || !vertex()[i].selected()) continue;

            vertexList.push_back(i);
            vertex().remove(i);
        }

        bool selected;
        int l;
        //loop though all triangles
        for(i = 0; i < triangle().size(); i++)
        {
            if(!triangle()[i].exists()) continue;

            selected = false;
            for(j = 0; j < 3; j++)
            {
                //check if this triangle's vertices were removed
                for(k = 0; k < vertexList.size(); k++)
                {
                    if(vertexList[k] != triangle()[i].getIndex(j)) continue;
                    //if there is one vertex removed

                    //add this triangle to list
                    triangleList.push_back(i);
                    selected = true;
                    //and other 2 vertices to vertexList2
                    for(l = 0; l < 3; l++) if(l != j) addToVertexList2(&vertexList, &vertexList2, triangle()[i].getIndex(l));
                    break;
                }
                if(selected) break;
            }
        }        
    }
    else
    {
        //add to triangleList all selected triangles
        for(i = 0; i < triangle().size(); i++)
        {
            if(!triangle()[i].exists() || !triangle()[i].selected()) continue;

            triangleList.push_back(i);
            //and all vertices of this triangle
            for(j = 0; j < 3; j++) addToVertexList2(&vertexList, &vertexList2, triangle()[i].getIndex(j));
        }

    }

    //remove triangles from list
    for(i = 0; i < triangleList.size(); i++) triangle().remove(triangleList[i]);

    //check if there are some existing triangles (when deleting, they are just marked as not existing)
    for(i = 0; i < triangle().size(); i++) if(triangle()[i].exists()) break;
    bool noTriangles = i == triangle().size();

    for(i = 0; i < vertexList2.size(); i++)
    {
        //vertex should be removed if it does not belong to any triangle
        //if there are no triangles
        if(noTriangles)
        {
            vertex().remove(vertexList2[i]);
            continue;
        }
        //if there are some, we should check if one of them is built on this vertex
        for(j = 0; j < triangle().size(); j++)
        {
            if(!triangle()[i].exists()) continue;

            for(k = 0; k < 3; k++) if(triangle()[j].getIndex(k) == vertexList2[i]) break;
            if(k < 3) break;
        }
        //if no such triangle found, remove vertex
        if(j == triangle().size()) vertex().remove(vertexList2[i]);
    }

   Journal::submit();
}



void MainWindow::addToVertexList2(vector <int> *vertexList, vector <int> *vertexList2, int index)
{
    //check if this vertex already is in vertexList 1 or 2
    for(int i = 0; i < vertexList->size(); i++) if((*vertexList)[i] == index) return;
    for(int i = 0; i < vertexList2->size(); i++) if((*vertexList2)[i] == index) return;
    vertexList2->push_back(index);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Shift:
    {
        if(activeTool()->shift()) activeTool()->shift()->setChecked(true);
        break;
    }
    case Qt::Key_Control:
    {
        if(activeTool()->ctrl()) activeTool()->ctrl()->setChecked(true);
        break;
    }
    case Qt::Key_1:
    {
        workWithElements[0]->setChecked(true);
        break;
    }
    case Qt::Key_2:
    {
        workWithElements[1]->setChecked(true);
        break;
    }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Shift:
    {
        if(activeTool()->shift()) activeTool()->shift()->setChecked(false);
        break;
    }
    case Qt::Key_Control:
    {
        if(activeTool()->ctrl()) activeTool()->ctrl()->setChecked(false);
        break;
    }
    }
}
