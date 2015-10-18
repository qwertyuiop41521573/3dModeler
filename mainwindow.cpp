#include "mainwindow.h"

#include "gui/myframe.h"


#include <iostream>

using namespace std;

MainWindow::MainWindow()
{
    int i;
    QWidget *centralWidget = new QWidget;

    model = new Model(&journal);
    journal.setModel(model);

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

    QWidget *workWithWidget = new QWidget;
    QGridLayout *workWithLayout = new QGridLayout;
    for(i = 0; i < 2; i++) workWithElements[i] = new QRadioButton;
    workWithElements[0]->setText("Vertex");
    workWithElements[1]->setText("Triangle");

    for(i = 0; i < 2; i++) workWithLayout->addWidget(workWithElements[i], i, 0);

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

    for(i = 0; i < 4; i++) hideViewportButtons[i] = new MyPushButton(i, this);

    maximizeButton = new QPushButton("Maximize\nActive");
    maximizeButton->setMaximumWidth(75);
    maximizeButton->setCheckable(true);
    connect(maximizeButton, SIGNAL(clicked(bool)), this, SLOT(maximize(bool)));

    MyFrame *line[5];
    for(i = 0; i < 5; i++) line[i] = new MyFrame;

    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

    scrollAreaLayout->addWidget(renderingModeLabel, 0, 0, 1, 4);
    scrollAreaLayout->addWidget(renderingMode, 1, 0, 1, 4);
    scrollAreaLayout->addWidget(wireframeOverlay, 2, 0, 1, 4);
    scrollAreaLayout->addWidget(projectionLabel, 3, 0, 1, 4);
    scrollAreaLayout->addWidget(projection, 4, 0, 1, 4);
    scrollAreaLayout->addWidget(viewportsLabel, 5, 0, 1, 4);

    for(i = 0; i < 4; i++) scrollAreaLayout->addWidget(hideViewportButtons[i], 6 + i / 2, i % 2);

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
    scrollAreaLayout->addWidget(line[3], 18, 0, 1, 4);
    scrollAreaLayout->addWidget(workWithWidget, 19, 0, 1, 4);
    scrollAreaLayout->addWidget(line[4], 20, 0, 1, 4);
    scrollAreaLayout->addWidget(tSelect->getWidget(), 21, 0, 1, 4);
    scrollAreaLayout->addWidget(tMove->getWidget(), 22, 0, 1, 4);
    scrollAreaLayout->addWidget(tScale->getWidget(), 23, 0, 1, 4);
    scrollAreaLayout->addWidget(tRotate->getWidget(), 24, 0, 1, 4);
    scrollAreaLayout->addWidget(tVertex->getWidget(), 25, 0, 1, 4);
    scrollAreaLayout->addWidget(tTriangle->getWidget(), 26, 0, 1, 4);
    scrollAreaLayout->addWidget(tPlane->getWidget(), 27, 0, 1, 4);
    scrollAreaLayout->addWidget(tBox->getWidget(), 28, 0, 1, 4);
    scrollAreaLayout->addWidget(tEllipse->getWidget(), 23, 0, 1, 4);
    scrollAreaLayout->addWidget(tCylinder->getWidget(), 24, 0, 1, 4);



    scrollAreaLayout->addItem(spacer, 40, 0, 1, 4);

    scrollAreaWidget->setLayout(scrollAreaLayout);
    scrollArea->setWidget(scrollAreaWidget);
        //scrollarea end

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

    for(i = 0; i < 4; i++) widget[i] = new GLWidget(this);

    widget[0]->setProjection(TOP);
    widget[1]->setProjection(FRONT);
    widget[2]->setProjection(LEFT);
    widget[3]->setProjection(PERSPECTIVE);

    widgetActive = widget[3];
    widget[3]->setRenderingMode(FLAT_SHADED);
    widget[3]->setWireframeOverlay(true);
    setActiveWidget(widget[3]);

    for(i = 0; i < 4; i++) viewportLayout->addWidget(widget[i], i / 2, i % 2);
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
    if(saveRequest())
    {
        if(openFileDialog("Open"))
        {
            if(!model->isEmpty) model->clear();
            model->load(model->fileName.toStdString().c_str());
        }
    }
}

void MainWindow::createActionsAndMenus()
{
    QAction *openAction = new QAction(tr("&Open"), this);
    openAction->setShortcut(tr("Ctrl+O"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    QAction *exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    QAction *newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
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
    editMenu->addAction(deleteAction);
}

void MainWindow::setActiveWidget(GLWidget *widget)
{
    widgetActive->setActive(false);
    widgetActive = widget;
    widgetActive->setActive(true);
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
    toolActive->setActive(false);
    toolActive = tool;
    tool->setActive(true);
}

bool MainWindow::saveRequest()
{
    if(!model->isEmpty)
    {
        if(model->isModified)
        {
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
                save();
                return true;
            }
            case QMessageBox::Discard:
                return true;
            }
            return false;
        }
    }
    return true;
}

void MainWindow::save()
{
    if(!model->loaded)
    {
        if(openFileDialog("Save")) model->save();
    }
    else model->save();
}

void MainWindow::selectAll()
{
    if(workWithElements[0]->isChecked()) for(int i = 0; i < model->getVertex().size(); i++) model->getVertex()[i].setSelected(true);
    else for(int i = 0; i < model->getTriangle().size(); i++) model->getTriangle()[i].setSelected(true);
}

void MainWindow::selectNone()
{
    if(workWithElements[0]->isChecked()) for(int i = 0; i < model->getVertex().size(); i++) model->getVertex()[i].setSelected(false);
    else for(int i = 0; i < model->getTriangle().size(); i++) model->getTriangle()[i].setSelected(false);
}

bool MainWindow::openFileDialog(QString action)
{
    QFileDialog dialog;
    QStringList filters;
    filters << "3dModeller (*.mdl)" << "All Files (*)";
    dialog.setNameFilters(filters);
    dialog.setWindowTitle(action + " Model");
    dialog.setLabelText(QFileDialog::Accept, action);
    bool result = dialog.exec();
    if(result)
    {
        dialog.selectedFiles();
        model->fileName = dialog.selectedFiles().join("");
    }
    return result;
}

void MainWindow::quickAccessToolOrbit()
{
    lastTool = toolActive;
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
    int i;
    vector <int> selected;
    QVector3D min, max;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexNumber = vertex.size();
    for(i = 0; i < vertexNumber; i++)
    {
        if(vertex[i].selected())
        {
            min = max = vertex[i].getPosition();
            selected.push_back(i);
            break;
        }
    }
    for( ; i < vertexNumber; i++)
    {
        if(model->getVertex()[i].selected())
        {
            selected.push_back(i);
            const QVector3D &vertex = model->getVertex()[i].getPosition();

            if(vertex.x() > max.x()) max.setX(vertex.x());
            if(vertex.y() > max.y()) max.setY(vertex.y());
            if(vertex.z() > max.z()) max.setZ(vertex.z());

            if(vertex.x() < min.x()) min.setX(vertex.x());
            if(vertex.y() < min.y()) min.setY(vertex.y());
            if(vertex.z() < min.z()) min.setZ(vertex.z());
        }
    }
    QVector3D center = (max + min) / 2;
    for(i = 0; i < selected.size(); i++) vertex[selected[i]].setPosition(center);
}

void MainWindow::deleteSlot()
{
    int i, j, k;
    vector <int> vertexList, triangleList, vertexList2;
    VertexContainer &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();

    journal.newRecord(REMOVE);

    bool chain;
    int end;
    if(workWithElements[0]->isChecked())
    {
        for(i = 0; i < vertex.size(); i++) if(vertex[i].selected())
        {
            vertexList.push_back(i);
            vertex.remove(i);
        }

        bool selected;
        int l;
        for(i = 0; i < triangle.size(); i++)
        {
            selected = false;
            for(j = 0; j < 3; j++)
            {
                for(k = 0; k < vertexList.size(); k++)
                {
                    if(vertexList[k] == triangle[i].getIndex(j))
                    {
                        triangleList.push_back(i);
                        selected = true;
                        for(l = 0; l < 3; l++) if(l != j) addToVertexList2(&vertexList, &vertexList2, triangle[i].getIndex(l));
                        break;
                    }
                }
                if(selected) break;
            }
        }        
    }
    else
    {
        for(i = 0; i < triangle.size(); i++) if(triangle[i].selected())
        {
            triangleList.push_back(i);
            for(j = 0; j < 3; j++) addToVertexList2(&vertexList, &vertexList2, triangle[i].getIndex(j));
        }

    }

    chain = false;
    for(i = triangleList.size() - 1; i > 0; i--)
    {
        if(triangleList[i] == triangleList[i - 1] - 1)
        {
            if(!chain) end = i;
            chain = true;
        }
        else
        {
            if(chain) triangle.erase(triangle.begin() + triangleList[i + 1], triangle.begin() + triangleList[end] + 1);
            else triangle.erase(triangle.begin() + triangleList[i]);
            chain = false;
        }
    }
    if(triangleList.size())
    {
        if(chain) triangle.erase(triangle.begin() + triangleList[0], triangle.begin() + triangleList[end] + 1);
        else triangle.erase(triangle.begin() + triangleList[0]);
    }

    for(i = 0; i < vertexList2.size(); i++)
    {
        if(triangle.size() == 0)
        {
            vertex.remove(vertexList2[i]);
            continue;
        }
        for(j = 0; j < triangle.size(); j++)
        {
            for(k = 0; k < 3; k++) if(triangle[j].getIndex(k) == vertexList2[i]) break;
            if(k < 3) break;
        }
        if(j == triangle.size()) vertex.remove(vertexList2[i]);
    }

    journal.submit();
}

Model *MainWindow::getModel()
{
    return model;
}

void MainWindow::stopQuickAccess()
{
    setActiveTool(lastTool);
};

void MainWindow::undo()
{
    if(journal.isEmpty()) return;
    const Record &rec = journal.current();
    int i;

    switch(rec.type())
    {
    case CREATE:
    {
        const vector <VertexAndIndex> &data = *rec.dataRO().createOrRemove;
        for(i = 0; i < data.size(); i++) model->getVertex()[data[i].index].remove();
        break;
    }
    case REMOVE:
    {
        const vector <VertexAndIndex> &data = *rec.dataRO().createOrRemove;
        for(i = 0; i < data.size(); i++) model->getVertex()[data[i].index] = data[i].vertex;
        break;
    }
    case SELECT:
    {
        const vector <SelValueAndIndex> &selVal = *rec.dataRO().select;
        for(i = 0; i < selVal.size(); i++) model->getVertex()[selVal[i].index].setSelected(!selVal[i].value);
        break;
    }
    }

    journal.undo();
}

void MainWindow::redo()
{
    if(journal.isFull()) return;
    const Record &rec = journal.next();

    int i;

    switch(rec.type())
    {
    case CREATE:
    {
        const vector <VertexAndIndex> &data = *rec.dataRO().createOrRemove;
        for(i = 0; i < data.size(); i++) model->getVertex()[data[i].index] = data[i].vertex;
        break;
    }
    case REMOVE:
    {
        const vector <VertexAndIndex> &data = *rec.dataRO().createOrRemove;
        for(i = 0; i < data.size(); i++) model->getVertex()[data[i].index].remove();
        break;
    }
    case SELECT:
    {
        const vector <SelValueAndIndex> &selVal = *rec.dataRO().select;
        for(i = 0; i < selVal.size(); i++) model->getVertex()[selVal[i].index].setSelected(selVal[i].value);
        break;
    }
    }

    journal.redo();
}

void MainWindow::addToVertexList2(vector <int> *vertexList, vector <int> *vertexList2, int index)
{
    for(int i = 0; i < vertexList->size(); i++) if((*vertexList)[i] == index) return;
    for(int i = 0; i < vertexList2->size(); i++) if((*vertexList2)[i] == index) return;
    vertexList2->push_back(index);
}
