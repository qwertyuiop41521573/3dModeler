#include "mainwindow.h"

#include "gui/myframe.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow()
{
    QWidget *centralWidget = new QWidget;

    Model::init(&journal);
    journal.setVariables(workWithElements);

    //tools
    tPan = new TPan(this);
    tZoom = new TZoom(this);
    tRotateCamera = new TRotateCamera(this);
    tOrbit = new TOrbit(this);

    tSelect = new TSelect(this);
    tMove = new TMove(this);
    tScale = new TScale(this);
    tRotate = new TRotate(this);
    tVertex = new TVertex(this);
    tTriangle = new TTriangle(this);
    tPlane = new TPlane(this);
    tBox = new TBox(this);
    tEllipse = new TEllipse(this);
    tCylinder = new TCylinder(this);
    tSphere = new TSphere(this);

    QWidget *workWithWidget = new QWidget;
    QGridLayout *workWithLayout = new QGridLayout;
    for(int i = 0; i < 2; i++) workWithElements[i] = new QRadioButton;
    workWithElements[0]->setText("Vertex");
    workWithElements[1]->setText("Triangle");

    for(int i = 0; i < 2; i++) workWithLayout->addWidget(workWithElements[i], i, 0);

    workWithElements[0]->setChecked(true);
    workWithWidget->setLayout(workWithLayout);


    toolActive = tSelect;
    setActiveTool(tSelect);

    //layouts and gui elements
    QGridLayout *centralLayout = new QGridLayout;

    //scrollarea
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFixedWidth(225);
    QWidget *scrollAreaWidget = new QWidget;
    scrollAreaWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    QGridLayout *scrollAreaLayout = new QGridLayout;

    renderingMode = new MyComboBox;

    renderingMode->addItem("Wireframe");
    renderingMode->addItem("Flat Shaded");
    renderingMode->addItem("Smooth Shaded");
    renderingMode->addItem("Textured");

    connect(renderingMode, SIGNAL(currentIndexChanged(int)), this, SLOT(changeRenderingMode(int)));

    QLabel *renderingModeLabel = new QLabel("Rendering Mode:");

    wireframeOverlay = new QCheckBox("Wireframe Overlay");

    connect(wireframeOverlay, SIGNAL(clicked(bool)), this, SLOT(changeWireFrameOverlay()));

    QLabel *projectionLabel = new QLabel("Projection:");
    projection = new MyComboBox;

    projection->addItem("Top");
    projection->addItem("Bottom");
    projection->addItem("Front");
    projection->addItem("Back");
    projection->addItem("Left");
    projection->addItem("Right");
    projection->addItem("Perpective");

    connect(projection, SIGNAL(currentIndexChanged(int)), this, SLOT(changeProjection(int)));

    QLabel *viewportsLabel = new QLabel("Viewports:");

    for(int i = 0; i < 4; i++) hideViewportButtons[i] = new MyPushButton(i, this);

    maximizeButton = new QPushButton("Maximize\nActive");
    maximizeButton->setMaximumWidth(75);
    maximizeButton->setCheckable(true);
    connect(maximizeButton, SIGNAL(clicked(bool)), this, SLOT(maximize(bool)));

    MyFrame *line[5];
    for(int i = 0; i < 5; i++) line[i] = new MyFrame;

    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    scrollAreaLayout->addWidget(renderingModeLabel, 0, 0, 1, 4);
    scrollAreaLayout->addWidget(renderingMode, 1, 0, 1, 4);
    scrollAreaLayout->addWidget(wireframeOverlay, 2, 0, 1, 4);
    scrollAreaLayout->addWidget(projectionLabel, 3, 0, 1, 4);
    scrollAreaLayout->addWidget(projection, 4, 0, 1, 4);
    scrollAreaLayout->addWidget(viewportsLabel, 5, 0, 1, 4);

    for(int i = 0; i < 4; i++) scrollAreaLayout->addWidget(hideViewportButtons[i], 6 + i / 2, i % 2);

    scrollAreaLayout->addWidget(maximizeButton, 6, 2, 2, 2);
    scrollAreaLayout->addWidget(line[0], 8, 0, 1, 4);
    scrollAreaLayout->addWidget(tPan->getButton(), 9, 0, 1, 2);
    scrollAreaLayout->addWidget(tZoom->getButton(), 9, 2, 1, 2);
    scrollAreaLayout->addWidget(tRotateCamera->getButton(), 10, 0, 1, 2);
    scrollAreaLayout->addWidget(tOrbit->getButton(), 10, 2, 1, 2);
    scrollAreaLayout->addWidget(line[1], 11, 0, 1, 4);
    scrollAreaLayout->addWidget(tSelect->getButton(), 12, 0, 1, 2);
    scrollAreaLayout->addWidget(tMove->getButton(), 12, 2, 1, 2);
    scrollAreaLayout->addWidget(tScale->getButton(), 13, 0, 1, 2);
    scrollAreaLayout->addWidget(tRotate->getButton(), 13, 2, 1, 2);
    scrollAreaLayout->addWidget(line[2], 14, 0, 1, 4);
    scrollAreaLayout->addWidget(tVertex->getButton(), 15, 0, 1, 2);
    scrollAreaLayout->addWidget(tTriangle->getButton(), 15, 2, 1, 2);
    scrollAreaLayout->addWidget(tPlane->getButton(), 16, 0, 1, 2);
    scrollAreaLayout->addWidget(tBox->getButton(), 16, 2, 1, 2);
    scrollAreaLayout->addWidget(tEllipse->getButton(), 17, 0, 1, 2);
    scrollAreaLayout->addWidget(tCylinder->getButton(), 17, 2, 1, 2);
    scrollAreaLayout->addWidget(tSphere->getButton(), 18, 0, 1, 2);
    scrollAreaLayout->addWidget(line[3], 19, 0, 1, 4);
    scrollAreaLayout->addWidget(workWithWidget, 20, 0, 1, 4);
    scrollAreaLayout->addWidget(line[4], 21, 0, 1, 4);
    scrollAreaLayout->addWidget(tSelect->getWidget(), 22, 0, 1, 4);
    scrollAreaLayout->addWidget(tMove->getWidget(), 23, 0, 1, 4);
    scrollAreaLayout->addWidget(tScale->getWidget(), 24, 0, 1, 4);
    scrollAreaLayout->addWidget(tRotate->getWidget(), 25, 0, 1, 4);
    scrollAreaLayout->addWidget(tVertex->getWidget(), 26, 0, 1, 4);
    scrollAreaLayout->addWidget(tTriangle->getWidget(), 27, 0, 1, 4);
    scrollAreaLayout->addWidget(tPlane->getWidget(), 28, 0, 1, 4);
    scrollAreaLayout->addWidget(tBox->getWidget(), 29, 0, 1, 4);
    scrollAreaLayout->addWidget(tEllipse->getWidget(), 30, 0, 1, 4);
    scrollAreaLayout->addWidget(tCylinder->getWidget(), 31, 0, 1, 4);
    scrollAreaLayout->addWidget(tSphere->getWidget(), 32, 0, 1, 4);

    scrollAreaLayout->addItem(spacer, 40, 0, 1, 4);

    scrollAreaWidget->setLayout(scrollAreaLayout);
    scrollArea->setWidget(scrollAreaWidget);
    //scrollarea end

    //tCylinder->widget() is not hidden as it is the biggest widget
    tCylinder->getWidget()->hide();
/*    tMove->getWidget()->hide();
    tScale->getWidget()->hide();
    tRotate->getWidget()->hide();
    tVertex->getWidget()->hide();
    tTriangle->getWidget()->hide();
    tPlane->getWidget()->hide();
    tBox->getWidget()->hide();
    tEllipse->getWidget()->hide();
    tCylinder->getWidget()->hide();*/

    //viewports
    QWidget *viewportWidget = new QWidget;
    QGridLayout *viewportLayout = new QGridLayout;

    for(int i = 0; i < 4; i++) widget[i] = new GLWidget(this);

    widget[0]->setProjection(TOP);
    widget[1]->setProjection(FRONT);
    widget[2]->setProjection(LEFT);
    widget[3]->setProjection(PERSPECTIVE);

    widgetActive = widget[3];
    widget[3]->setRenderingMode(FLAT_SHADED);
    widget[3]->setWireframeOverlay(true);
    setActiveWidget(widget[3]);

    for(int i = 0; i < 4; i++) viewportLayout->addWidget(widget[i], i / 2, i % 2);
    viewportWidget->setLayout(viewportLayout);
    //viewports end

    //add viewports and scrollarea to window
    centralLayout->addWidget(viewportWidget, 0, 0);
    centralLayout->addWidget(scrollArea, 0, 1);
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);

    createActionsAndMenus();

    setWindowTitle("3d Modeler");

 //  model->load( "/path/to/model.mdl" );
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
    QAction *openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(tr("Ctrl+O"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    QAction *exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(handleClose()));
    QAction *newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newModel()));
    QAction *saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    QAction *saveAsAction = new QAction(tr("&Save As"), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    QAction *undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+Z"));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    QAction *redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcut(tr("Ctrl+Shift+Z"));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));
    QAction *selectAllAction = new QAction(tr("&Select All"), this);
    selectAllAction->setShortcut(tr("Ctrl+A"));
    connect(selectAllAction, SIGNAL(triggered()), this, SLOT(selectAll()));
    QAction *selectNoneAction = new QAction(tr("&Select None"), this);
    selectNoneAction->setShortcut(tr("Ctrl+D"));
    connect(selectNoneAction, SIGNAL(triggered()), this, SLOT(selectNone()));
    QAction *snapTogetherAction = new QAction(tr("&Snap Together"), this);
    snapTogetherAction->setShortcut(tr("Ctrl+T"));
    connect(snapTogetherAction, SIGNAL(triggered()), this, SLOT(snapTogether()));
    QAction *weldTogetherAction = new QAction(tr("&Weld Together"), this);
    weldTogetherAction->setShortcut(tr("Ctrl+W"));
    connect(weldTogetherAction, SIGNAL(triggered()), this, SLOT(weldTogether()));
    QAction *deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(tr("Del"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteSlot()));


    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(exitAction);

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addAction(selectAllAction);
    editMenu->addAction(selectNoneAction);
    editMenu->addAction(snapTogetherAction);
    editMenu->addAction(weldTogetherAction);
    editMenu->addAction(deleteAction);
}

void MainWindow::setActiveWidget(GLWidget *widget)
{
    //deactivate current active widget
    widgetActive->setActive(false);
    //copy new widget pointer to widgetActive
    widgetActive = widget;
    //activate new active widget
    widgetActive->setActive(true);
    //load some parameters from new active widget
    renderingMode->setCurrentIndex(widgetActive->getRenderingMode());
    wireframeOverlay->setChecked(widgetActive->getWireframeOverlay());
    projection->setCurrentIndex(widgetActive->getProjection());
}

void MainWindow::setActiveTool(Tool *tool)
{
    if(toolActive->hasStage2() && toolActive->stage2())
    {
        tool->getButton()->setChecked(false);
        return;
    }
    //deactivate current
    toolActive->setActive(false);
    //copy pointer to toolActive
    toolActive = tool;
    //activate new
    tool->setActive(true);
}

void MainWindow::selectAll()
{
    using namespace Model;
    journal.newRecord(EDIT);
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
    journal.submit();
}

void MainWindow::selectNone()
{
    using namespace Model;
    journal.newRecord(EDIT);
    if(workWithElements[0]->isChecked())
    {
        for(int i = 0; i < vertex().size(); i++) if(vertex()[i].exists()) vertex().setSelected(i, false);
    }
    else
    {
        for(int i = 0; i < triangle().size(); i++) if(triangle()[i].exists()) triangle().setSelected(i, false);
    }   
    journal.submit();
}

void MainWindow::quickAccessToolOrbit()
{
    //backup current active too
    lastTool = toolActive;
    //and switch to tOrbit
    setActiveTool(tOrbit);
}

void MainWindow::quickAccessToolPan()
{
    lastTool = toolActive;
    setActiveTool(tPan);
}

void MainWindow::hideViewport(int index)
{
    widget[index]->setHidden(!widget[index]->isHidden());
    //if three viewports are hidden, it's equal to maximizing one viewport
    bool threeViewportsHidden = true;
    for(int i = 0; i < 4; i++) if(widget[i] != widgetActive) threeViewportsHidden *= hideViewportButtons[i]->isChecked();
    maximizeButton->setChecked(threeViewportsHidden && !widgetActive->isHidden());
}

void MainWindow::maximize(bool value)
{
    if(value)
    {
        for(int i = 0; i < 4; i++)
        {
            widget[i]->setOldHidden(widget[i]->isHidden());
            hideViewportButtons[i]->setChecked(!(widget[i] == widgetActive));
        }
    }
    else for(int i = 0; i < 4; i++) hideViewportButtons[i]->setChecked(widget[i]->oldHidden());
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
    journal.newRecord(EDIT);
    for(i = 0; i < selected.size(); i++)
    {
        journal.addBefore(true, selected[i]);
        //move to center
        vertex()[selected[i]].setPosition(center);
        journal.addAfter(true);
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
        journal.newRecord(EDIT);
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
            journal.addBefore(true, index);
            //remove vertex
            vertex().remove(index);
            journal.addAfter(true);
            //replace this vertex with first from group in all triangles
            for(k = 0; k < triangle().size(); k++)
            {
                if(!triangle()[k].exists()) continue;

                for(l = 0; l < 3; l++)
                {
                    if(triangle()[k].getIndex(l) == index)
                    {
                        journal.addBefore(false, k);
                        triangle()[k].setIndex(l, groups[i][0]);
                        journal.addAfter(false);
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

    journal.newRecord(EDIT);

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

   journal.submit();
}

void MainWindow::stopQuickAccess()
{
    setActiveTool(lastTool);
}

void MainWindow::undo()
{
    if(toolActive->busy() || journal.isEmpty()) return;

    const Record &rec = journal.currentRO();
    int i;

    switch(rec.type())
    {
    case CREATE:
    {
        const Create &data = *rec.dataRO().create;
        const vector <ElementWithIndex <Vertex> > &vertex = data.verRO();
        //remove what was created
        for(i = 0; i < vertex.size(); i++) Model::vertex()[vertex[i].index()].remove();
        const vector <ElementWithIndex <Triangle> > &triangle = data.triRO();
        for(i = 0; i < triangle.size(); i++) Model::triangle()[triangle[i].index()].remove();
        break;
    }
    case EDIT:
    {
        const Edit &data = *rec.dataRO().edit;
        //replace edited elements with their "before" value
        const vector <TwoElementsWithIndex <Vertex> > &vertex = data.verRO();
        for(i = 0; i < vertex.size(); i++) Model::vertex()[vertex[i].index()] = vertex[i].before();
        const vector <TwoElementsWithIndex <Triangle> > &triangle = data.triRO();
        for(i = 0; i < triangle.size(); i++) Model::triangle()[triangle[i].index()] = triangle[i].before();

        break;
    }
    }

    journal.undo();
}

void MainWindow::redo()
{
    if(toolActive->busy() || journal.isFull()) return;
    const Record &rec = journal.next();

    int i;

    switch(rec.type())
    {
    case CREATE:
    {
        const Create &data = *rec.dataRO().create;
        //recreate elements
        const vector <ElementWithIndex <Vertex> > &vertex = data.verRO();
        for(i = 0; i < vertex.size(); i++) Model::vertex()[vertex[i].index()] = vertex[i].valRO();
        const vector <ElementWithIndex <Triangle> > &triangle = data.triRO();
        for(i = 0; i < triangle.size(); i++) Model::triangle()[triangle[i].index()] = triangle[i].valRO();
        break;
    }
    case EDIT:
    {
        const Edit &data = *rec.dataRO().edit;
        //replace edited elements with their "after" value
        const vector <TwoElementsWithIndex <Vertex> > &vertex = data.verRO();
        for(i = 0; i < vertex.size(); i++) Model::vertex()[vertex[i].index()] = vertex[i].after();
        const vector <TwoElementsWithIndex <Triangle> > &triangle = data.triRO();
        for(i = 0; i < triangle.size(); i++) Model::triangle()[triangle[i].index()] = triangle[i].after();

        break;
    }
    }

    journal.redo();
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
        if(toolActive->shift()) toolActive->shift()->setChecked(true);
        break;
    }
    case Qt::Key_Control:
    {
        if(toolActive->ctrl()) toolActive->ctrl()->setChecked(true);
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
        if(toolActive->shift()) toolActive->shift()->setChecked(false);
        break;
    }
    case Qt::Key_Control:
    {
        if(toolActive->ctrl()) toolActive->ctrl()->setChecked(false);
        break;
    }
    }
}
