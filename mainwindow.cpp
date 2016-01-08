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
{ if(Model::modified() ? saveRequest() : true) close(); }

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

    selectAllAction = new QAction(tr("&Select All"), this);
    selectAllAction->setShortcut(tr("Ctrl+A"));
    selectNoneAction = new QAction(tr("&Select None"), this);
    selectNoneAction->setShortcut(tr("Ctrl+D"));
    snapTogetherAction = new QAction(tr("&Snap Together"), this);
    snapTogetherAction->setShortcut(tr("Ctrl+T"));
    weldTogetherAction = new QAction(tr("&Weld Together"), this);
    weldTogetherAction->setShortcut(tr("Ctrl+W"));
    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setShortcut(tr("Del"));

    Journal::connectActions(undoAction, redoAction);
    ToolSet::connectActions(selectAllAction, selectNoneAction, snapTogetherAction, weldTogetherAction, deleteAction);
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
