#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.h"
#include "toolset.h"
#include "workspace.h"

#include "gui/mypushbutton.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void open();
    void newModel();
    bool save();
    void saveAs();
    void handleClose();

private:
    QWidget *workWithWidget;

    QAction *openAction;
    QAction *exitAction;
    QAction *newAction;
    QAction *saveAction;
    QAction *saveAsAction;

    QAction *undoAction;
    QAction *redoAction;
    QAction *selectAllAction;
    QAction *selectNoneAction;
    QAction *snapTogetherAction;
    QAction *weldTogetherAction;
    QAction *deleteAction;


    void initMainWindow();

    void createCentralWidget();
    QGridLayout *createCentralLayout();

    QScrollArea *createScrollArea();
    QWidget *createScrollAreaWidget();

    QGridLayout *createScrollAreaLayout();

    QWidget *createViewportsWidget();
    QGridLayout *createViewportsLayout();
    void createViewports();

    void createActionsAndMenus();

    void createActions();
    void fileActions();
    void editActions();
    void createMenus();
    void fileMenu();
    void editMenu();

    bool saveRequest();
    bool openFileDialog(QString title);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
