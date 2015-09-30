#include <QtGui>
#include <QtOpenGL>
#include <QSpacerItem>

#include "mainwindow.h"

#include <iostream>

using namespace std;

MainWindow::MainWindow()
{
    int i;
    centralWidget = new QWidget;

    model = new Model;


    // TOOLS

    tPan = new TPan(this);
    tZoom = new TZoom(this);
    tRotateCamera = new TRotateCamera(this);
    tOrbit = new TOrbit(this);

    //toolSelect
    QWidget *toolSelectWidget = new QWidget;
    {
        QGridLayout *toolSelectLayout = new QGridLayout;
        WidgetElements *toolSelectElements = new
                WidgetElements( 0, 0, 0, 2, 0 );

        toolSelectElements->getMyCheckBox( 0 )->
                setText( "Add" );
        toolSelectElements->getMyCheckBox( 1 )->
                setText( "Remove" );

        for( i = 0; i < 2; i++ )
        {
            connect( toolSelectElements->getMyCheckBox( i ), SIGNAL(
                    handledClick( MyCheckBox* ) ), this, SLOT(
                    handleSelectClick( MyCheckBox* ) ) );
            toolSelectLayout->addWidget( toolSelectElements->
                                        getMyCheckBox( i ), 0, i );
        }

        toolSelectWidget->setLayout( toolSelectLayout );
        tSelect = new TSelect(this, toolSelectWidget,
                              toolSelectElements);
    }


    //toolMove
    QWidget *toolMoveWidget = new QWidget;
    {
        QGridLayout *toolMoveLayout = new QGridLayout;
        WidgetElements *toolMoveElements = new
                WidgetElements( 3, 3, 0, 0, 0 );

        toolMoveElements->getPushButton( 0 )->setText( "X" );
        toolMoveElements->getPushButton( 1 )->setText( "Y" );
        toolMoveElements->getPushButton( 2 )->setText( "Z" );

        for( i = 0; i < 3; i++ )
        {
            toolMoveLayout->addWidget(
                        toolMoveElements->getPushButton( i ), i, 0 );
            toolMoveLayout->addWidget(
                        toolMoveElements->getSpinBox( i ), i, 1 );
        }

        toolMoveWidget->setLayout( toolMoveLayout );
        tMove = new TMove(this, toolMoveWidget,
                          toolMoveElements, "Move");
        toolMoveLayout->addWidget( tMove->getFinalButton(), 3, 0,
                                   1, 2 );

        connect( tMove->getFinalButton(), SIGNAL( clicked() ),
                 this, SLOT( final() ) );
    }

    //toolScale
    QWidget *toolScaleWidget = new QWidget;
    {
        QGridLayout *toolScaleLayout = new QGridLayout;
        WidgetElements *toolScaleElements = new
                WidgetElements( 3, 3, 0, 0, 1 );

        toolScaleElements->getPushButton( 0 )->setText( "X" );
        toolScaleElements->getPushButton( 1 )->setText( "Y" );
        toolScaleElements->getPushButton( 2 )->setText( "Z" );

        for( i = 0; i < 3; i++ )
        {
            toolScaleLayout->addWidget(
                        toolScaleElements->getPushButton( i ), i, 0 );
            toolScaleLayout->addWidget(
                        toolScaleElements->getSpinBox( i ), i, 1 );
        }


        toolScaleWidget->setLayout( toolScaleLayout );
        tScale = new TScale(this, toolScaleWidget,
                            toolScaleElements, "Scale");
        toolScaleLayout->addWidget( tScale->getFinalButton(), 3,
                                    0, 1, 2 );

        connect( tScale->getFinalButton(), SIGNAL( clicked() ),
                 this, SLOT( final() ) );
    }

    //toolRotate
    QWidget *toolRotateWidget = new QWidget;
    {
        QGridLayout *toolRotateLayout = new QGridLayout;
        WidgetElements *toolRotateElements = new WidgetElements(
                    0, 4, 0, 1, 0 );

        QLabel *toolRotateLabels[ 4 ];
        toolRotateLabels[ 0 ] = new QLabel( "Angle:" );
        for( i = 1; i < 4; i++ ) toolRotateLabels[ i ] = new QLabel(
                        QString( 'W' + i ) + ':' );
        for( i = 0; i < 4; i++ )
        {
            toolRotateLabels[ i ]->setMaximumWidth( 50 );
            toolRotateLabels[ i ]->setAlignment( Qt::AlignRight );
            toolRotateLayout->addWidget( toolRotateLabels[ i ], i, 0 );
            toolRotateLayout->addWidget( toolRotateElements->
                                      getSpinBox( i ), i, 1 );
        }
        toolRotateElements->getSpinBox( 0 )->setMaximum( 360 );
        toolRotateElements->getSpinBox( 0 )->setMinimum(-360 );

        toolRotateElements->getMyCheckBox( 0 )->setText( "Custom Axis" );
        toolRotateElements->getMyCheckBox( 0 )->setChecked( true );
        toolRotateLayout->addWidget( toolRotateElements->
                                     getMyCheckBox( 0 ), 5, 0, 1, 2 );
        toolRotateWidget->setLayout( toolRotateLayout );
        tRotate = new TRotate(this, toolRotateWidget, toolRotateElements,
                               "Rotate" );
        toolRotateLayout->addWidget( tRotate->getFinalButton(), 4,
                                     0, 1, 2 );

        connect( tRotate->getFinalButton(), SIGNAL( clicked() ),
                 this, SLOT( final() ) );
    }

    //toolVertex
    QWidget *toolVertexWidget = new QWidget;
    {
        QGridLayout *toolVertexLayout = new QGridLayout;
        WidgetElements *toolVertexElements = new WidgetElements( 0, 3,
                                                                 0, 0, 0 );
        QLabel *toolVertexLabels[ 3 ];
        for( i = 0; i < 3; i++ ) toolVertexLabels[ i ] = new QLabel(
                        QString( 'X' + i ) + ':' );
        for( i = 0; i < 3; i++ )
        {
            toolVertexLabels[ i ]->setMaximumWidth( 50 );
            toolVertexLabels[ i ]->setAlignment( Qt::AlignRight );
            toolVertexLayout->addWidget( toolVertexLabels[ i ], i, 0 );
            toolVertexLayout->addWidget( toolVertexElements->
                                      getSpinBox( i ), i, 1 );
        }

        toolVertexWidget->setLayout( toolVertexLayout );
        toolVertex = new ToolWithWidget(this,
                toolVertexWidget, toolVertexElements,
                               "Create Vertex" );
        toolVertexLayout->addWidget( toolVertex->getFinalButton(),
                                     3, 0, 1, 2 );

        connect( toolVertex->getFinalButton(), SIGNAL( clicked() ),
                 this, SLOT( final() ) );
    }

    //toolTriangle
    QWidget *toolTriangleWidget = new QWidget;
    {
        QGridLayout *toolTriangleLayout = new QGridLayout;
        WidgetElements *toolTriangleElements = new WidgetElements( 0, 0,
                                                                   0, 0, 0 );
        toolTriangleWidget->setLayout( toolTriangleLayout );
        toolTriangle = new ToolWithWidget(this,
             toolTriangleWidget, toolTriangleElements,
                                 "Cancel" );
        toolTriangleLayout->addWidget( toolTriangle->getFinalButton(),
                                       0, 0 );

        connect( toolTriangle->getFinalButton(), SIGNAL( clicked() ),
                 this, SLOT( final() ) );
    }

    //toolPlane
    QWidget *toolPlaneWidget = new QWidget;
    {
        QGridLayout *toolPlaneLayout = new QGridLayout;
        WidgetElements *toolPlaneElements = new WidgetElements( 0, 12, 0, 0, 1 );
        toolPlaneElements->getCheckBox( 0 )->setText( "Square" );
        toolPlaneElements->getCheckBox( 0 )->setMaximumWidth( 130 );
        toolPlaneLayout->addWidget( toolPlaneElements->getCheckBox( 0 ), 0, 0, 1, 6 );
        QLabel *toolPlaneLabels[ 12 ];
        for( i = 0; i < 12; i++ )
        {
            toolPlaneLabels[ i ] = new QLabel( QString( 'X' + i % 3 )
                                               + ':' );
            toolPlaneLabels[ i ]->setMaximumWidth( 25 );
            toolPlaneLabels[ i ]->setAlignment( Qt::AlignRight );
            int x = 4 * ( ( i % 6 ) / 3 ), y = 4 * ( i / 6 ) + i % 3 + 1;
            toolPlaneLayout->addWidget( toolPlaneLabels[ i ], y, x );
            toolPlaneElements->getSpinBox( i )->setMaximumWidth( 50 );
            toolPlaneLayout->addWidget( toolPlaneElements->getSpinBox( i ), y, x + 1 );
        }
        QFrame *toolPlaneHLine = new QFrame();
        toolPlaneHLine->setFrameShape( QFrame::HLine );
        toolPlaneHLine->setFrameShadow( QFrame::Sunken );
        toolPlaneHLine->setMaximumWidth( 150 );
        toolPlaneHLine->setFixedHeight( 5 );
        toolPlaneLayout->addWidget( toolPlaneHLine, 4, 0, 1, 6 );
        QFrame *toolPlaneVLine = new QFrame();
        toolPlaneVLine->setFrameShape( QFrame::VLine );
        toolPlaneVLine->setFrameShadow( QFrame::Sunken );
        toolPlaneVLine->setMaximumHeight( 200 );
        toolPlaneVLine->setFixedWidth( 5 );
        toolPlaneLayout->addWidget( toolPlaneVLine, 1, 3, 7, 1 );

        toolPlaneWidget->setLayout( toolPlaneLayout );
        toolPlane = new ToolWithWidget(this,
                            toolPlaneWidget, toolPlaneElements,
                              "Create Plane" );
        toolPlaneLayout->addWidget( toolPlane->getFinalButton(), 9,
                                    0, 1, 6 );

        connect( toolPlane->getFinalButton(), SIGNAL( clicked() ),
                 this, SLOT( final() ) );
    }

    //toolBox
    QWidget *toolBoxWidget = new QWidget;
    {
        QGridLayout *toolBoxLayout = new QGridLayout;
        WidgetElements *toolBoxElements = new WidgetElements( 0, 6, 0, 0, 2 );
        toolBoxWidget->setLayout( toolBoxLayout );
        toolBox = new ToolWithWidget(this, toolBoxWidget, toolBoxElements,
                            "Create Box", true );
        toolBoxElements->getCheckBox( 0 )->setText( "Square" );
        toolBoxElements->getCheckBox( 0 )->setMaximumWidth( 130 );
        toolBoxElements->getCheckBox( 1 )->setText( "Cube" );
        toolBoxElements->getCheckBox( 1 )->setMaximumWidth( 130 );
        QLabel *toolBoxCenter = new QLabel( "Center" );
        QLabel *toolBoxSize = new QLabel( "Size" );
        QLabel *toolBoxLabels[ 6 ];
        for( i = 0; i < 6; i++ )
        {
            toolBoxLabels[ i ] = new QLabel( QString( 'X' + i % 3 )
                                               + ':' );
            toolBoxLabels[ i ]->setMaximumWidth( 25 );
            toolBoxLabels[ i ]->setAlignment( Qt::AlignRight );
            toolBoxElements->getSpinBox( i )->setMaximumWidth( 50 );
        }
        toolBoxLayout->addWidget( toolBoxElements->getCheckBox( 0 ), 0, 0, 1, 4 );
        toolBoxLayout->addWidget( toolBoxElements->getCheckBox( 1 ), 1, 0, 1, 4 );
        toolBoxLayout->addWidget( toolBoxCenter, 2, 0, 1, 4 );
        toolBoxLayout->addWidget( toolBoxSize, 2, 2, 1, 4 );
        for( i = 0; i < 3; i++ )
        {
            toolBoxLayout->addWidget( toolBoxLabels[ i ], 3 + i, 0 );
            toolBoxLayout->addWidget( toolBoxElements->getSpinBox( i ), 3 + i, 1 );
            toolBoxLayout->addWidget( toolBoxLabels[ i + 3 ], 3 + i, 2 );
            toolBoxLayout->addWidget( toolBoxElements->getSpinBox( i + 3 ), 3 + i, 3 );
        }
        toolBoxLayout->addWidget( toolBox->getFinalButton(), 6, 0, 1, 4 );

        connect( toolBox->getFinalButton(), SIGNAL( clicked() ), this, SLOT( final() ) );
    }

    //toolEllipse
    QWidget *toolEllipseWidget = new QWidget;
    {
        QGridLayout *toolEllipseLayout = new QGridLayout;
        WidgetElements *toolEllipseElements = new WidgetElements( 0, 8, 0, 0, 1 );
        toolEllipseWidget->setLayout( toolEllipseLayout );
        toolEllipse = new ToolWithWidget(this, toolEllipseWidget, toolEllipseElements, "Create Ellipse" );

        toolEllipseElements->getSpinBox( 0 )->setMaximumWidth( 50 );
        toolEllipseElements->getSpinBox( 0 )->setMinimum( 3 );
        toolEllipseElements->getSpinBox( 0 )->setValue( 18 );
        QLabel *toolEllipseSegments = new QLabel( "Segments:" );
        toolEllipseSegments->setAlignment( Qt::AlignRight );
        toolEllipseSegments->setMaximumWidth( 70 );

        toolEllipseElements->getCheckBox( 0 )->setText( "Circle" );
        toolEllipseElements->getCheckBox( 0 )->setMaximumWidth( 130 );

        QLabel *toolEllipseCenter = new QLabel( "Center" );
        QLabel *toolEllipseNormal = new QLabel( "Normal" );
        QLabel *toolEllipseLabels[ 6 ];
        for( i = 0; i < 6; i++ )
        {
            toolEllipseLabels[ i ] = new QLabel( QString( 'X' + i % 3 ) + ':' );
            toolEllipseLabels[ i ]->setMaximumWidth( 25 );
            toolEllipseLabels[ i ]->setAlignment( Qt::AlignRight );
            toolEllipseElements->getSpinBox( 1 + i )->setMaximumWidth( 50 );
        }
        QLabel *toolEllipseRadius = new QLabel( "Radius:" );
        toolEllipseRadius->setAlignment( Qt::AlignRight );
        toolEllipseRadius->setMaximumWidth( 70 );

        toolEllipseElements->getSpinBox( 7 )->setMinimum( 0 );
        toolEllipseElements->getSpinBox( 7 )->setValue( 1 );

        toolEllipseLayout->addWidget( toolEllipseSegments, 0, 0, 1, 2 );
        toolEllipseLayout->addWidget( toolEllipseElements->getSpinBox( 0 ), 0, 2, 1, 2 );
        toolEllipseLayout->addWidget( toolEllipseElements->getCheckBox( 0 ), 1, 0, 1, 4 );
        toolEllipseLayout->addWidget( toolEllipseCenter, 2, 0, 1, 4 );
        toolEllipseLayout->addWidget( toolEllipseNormal, 2, 2, 1, 4 );
        for( i = 0; i < 3; i++ )
        {
            toolEllipseLayout->addWidget( toolEllipseLabels[ i ], 3 + i, 0 );
            toolEllipseLayout->addWidget( toolEllipseElements->getSpinBox( 1 + i ), 3 + i, 1 );
            toolEllipseLayout->addWidget( toolEllipseLabels[ i + 3 ], 3 + i, 2 );
            toolEllipseLayout->addWidget( toolEllipseElements->getSpinBox( 4 + i ), 3 + i, 3 );
        }
        toolEllipseLayout->addWidget( toolEllipseRadius, 6, 0, 1, 2 );
        toolEllipseLayout->addWidget( toolEllipseElements->getSpinBox( 7 ), 6, 2, 1, 2 );
        toolEllipseLayout->addWidget( toolEllipse->getFinalButton(), 7, 0, 1, 4 );



        connect( toolEllipse->getFinalButton(), SIGNAL( clicked() ), this, SLOT( final() ) );
    }

    //toolCylinder
    QWidget *toolCylinderWidget = new QWidget;
    {
        QGridLayout *toolCylinderLayout = new QGridLayout;
        WidgetElements *toolCylinderElements = new WidgetElements( 0, 8, 0, 0, 1 );
        toolCylinderWidget->setLayout( toolCylinderLayout );
        toolCylinder = new ToolWithWidget(this, toolCylinderWidget,
                                 toolCylinderElements, "Create Cylinder", true );

        toolCylinderElements->getSpinBox( 0 )->setMaximumWidth( 50 );
        toolCylinderElements->getSpinBox( 0 )->setMinimum( 3 );
        toolCylinderElements->getSpinBox( 0 )->setValue( 18 );
        QLabel *toolCylinderSegments = new QLabel( "Segments:" );
        toolCylinderSegments->setAlignment( Qt::AlignRight );
        toolCylinderSegments->setMaximumWidth( 70 );

        toolCylinderElements->getCheckBox( 0 )->setText( "Circle" );
        toolCylinderElements->getCheckBox( 0 )->setMaximumWidth( 130 );

        QLabel *toolCylinderCenter = new QLabel( "Center" );
        QLabel *toolCylinderNormal = new QLabel( "Normal" );
        QLabel *toolCylinderLabels[ 6 ];
        for( i = 0; i < 6; i++ )
        {
            toolCylinderLabels[ i ] = new QLabel( QString( 'X' + i % 3 ) + ':' );
            toolCylinderLabels[ i ]->setMaximumWidth( 25 );
            toolCylinderLabels[ i ]->setAlignment( Qt::AlignRight );
            toolCylinderElements->getSpinBox( 1 + i )->setMaximumWidth( 50 );
        }
        QLabel *toolCylinderRadius = new QLabel( "Radius:" );
        toolCylinderRadius->setAlignment( Qt::AlignRight );
        toolCylinderRadius->setMaximumWidth( 70 );

        toolCylinderElements->getSpinBox( 7 )->setMinimum( 0 );
        toolCylinderElements->getSpinBox( 7 )->setValue( 1 );

        toolCylinderLayout->addWidget( toolCylinderSegments, 0, 0, 1, 2 );
        toolCylinderLayout->addWidget( toolCylinderElements->getSpinBox( 0 ), 0, 2, 1, 2 );
        toolCylinderLayout->addWidget( toolCylinderElements->getCheckBox( 0 ), 1, 0, 1, 4 );
        toolCylinderLayout->addWidget( toolCylinderCenter, 2, 0, 1, 4 );
        toolCylinderLayout->addWidget( toolCylinderNormal, 2, 2, 1, 4 );
        for( i = 0; i < 3; i++ )
        {
            toolCylinderLayout->addWidget( toolCylinderLabels[ i ], 3 + i, 0 );
            toolCylinderLayout->addWidget( toolCylinderElements->getSpinBox( 1 + i ), 3 + i, 1 );
            toolCylinderLayout->addWidget( toolCylinderLabels[ i + 3 ], 3 + i, 2 );
            toolCylinderLayout->addWidget( toolCylinderElements->getSpinBox( 4 + i ), 3 + i, 3 );
        }
        toolCylinderLayout->addWidget( toolCylinderRadius, 6, 0, 1, 2 );
        toolCylinderLayout->addWidget( toolCylinderElements->getSpinBox( 7 ), 6, 2, 1, 2 );
        toolCylinderLayout->addWidget( toolCylinder->getFinalButton(), 7, 0, 1, 4 );



        connect( toolCylinder->getFinalButton(), SIGNAL( clicked() ), this, SLOT( final() ) );
    }

    QWidget *workWithWidget = new QWidget;
    QGridLayout *workWithLayout = new QGridLayout;
    workWithElements = new
            WidgetElements( 0, 0, 2, 0, 0 );

    workWithElements->getRadioButton( 0 )->setText( "Vertex" );
    workWithElements->getRadioButton( 1 )->setText( "Triangle" );

    for( i = 0; i < 2; i++ )
        workWithLayout->addWidget( workWithElements->
                                getRadioButton( i ), i, 0 );


    workWithElements->getRadioButton( 0 )->setChecked( true );

    workWithWidget->setLayout( workWithLayout );


    toolActive = tSelect;
    setActiveTool(tSelect);

    // TOOLS END


    //layouts and gui elements
    QGridLayout *centralLayout = new QGridLayout;

    //scrollarea
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setHorizontalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setFixedWidth(190);
    QWidget *scrollAreaWidget = new QWidget;
    QGridLayout *scrollAreaLayout = new QGridLayout;

    renderingMode = new MyComboBox;

    renderingMode->addItem("Wireframe");
    renderingMode->addItem("Flat Shaded");
    renderingMode->addItem("Smooth Shaded");
    renderingMode->addItem("Textured");

    connect(renderingMode, SIGNAL(currentIndexChanged(int)), this,
            SLOT(changeRenderingMode(int)));

    QLabel *renderingModeLabel = new QLabel("Rendering Mode:");

    wireframeOverlay = new QCheckBox("Wireframe Overlay");

    connect(wireframeOverlay, SIGNAL(clicked(bool)), this, SLOT(
                changeWireFrameOverlay()));

    QLabel *projectionLabel = new QLabel("Projection:");
    projection = new MyComboBox;

    projection->addItem("Top");
    projection->addItem("Bottom");
    projection->addItem("Front");
    projection->addItem("Back");
    projection->addItem("Left");
    projection->addItem("Right");
    projection->addItem("Perpective");

    connect(projection, SIGNAL(currentIndexChanged(int)), this,
            SLOT(changeProjection(int)));

    QLabel *viewportsLabel = new QLabel("Viewports:");

    for(i = 0; i < 4; i++) hideViewportButtons[i] = new
            MyPushButton(i, this);

    maximizeButton = new QPushButton("Maximize\nActive");
    maximizeButton->setMaximumWidth(75);
    maximizeButton->setCheckable(true);
    connect(maximizeButton, SIGNAL(clicked(bool)), this, SLOT(
                maximize(bool)));

    QFrame *line[5];
    for(i = 0; i < 5; i++)
    {
        line[i] = new QFrame;
        line[i]->setFrameShape(QFrame::HLine);
        line[i]->setFrameShadow(QFrame::Sunken);
        line[i]->setMaximumWidth(150);
    }

    QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::
                                   Minimum, QSizePolicy::Expanding);

    scrollAreaLayout->addWidget(renderingModeLabel, 0, 0, 1, 4);
    scrollAreaLayout->addWidget(renderingMode, 1, 0, 1, 4);
    scrollAreaLayout->addWidget(wireframeOverlay, 2, 0, 1, 4);
    scrollAreaLayout->addWidget(projectionLabel, 3, 0, 1, 4);
    scrollAreaLayout->addWidget(projection, 4, 0, 1, 4);
    scrollAreaLayout->addWidget(viewportsLabel, 5, 0, 1, 4);

    for(i = 0; i < 4; i++) scrollAreaLayout->addWidget(
                hideViewportButtons[i], 6 + i / 2, i % 2);

    scrollAreaLayout->addWidget(maximizeButton, 6, 2, 2, 2);
    scrollAreaLayout->addWidget(line[0], 8, 0, 1, 4);
    scrollAreaLayout->addWidget(tPan->getButton(), 9, 0, 1, 2);
    scrollAreaLayout->addWidget(tZoom->getButton(), 9, 2, 1, 2);
    scrollAreaLayout->addWidget(tRotateCamera->getButton(),
                                 10, 0, 1, 2);
    scrollAreaLayout->addWidget(tOrbit->getButton(), 10, 2, 1,
                                2);
    scrollAreaLayout->addWidget(line[1], 11, 0, 1, 4);
    scrollAreaLayout->addWidget(tSelect->getButton(), 12, 0,
                                 1, 2);
    scrollAreaLayout->addWidget(tMove->getButton(), 12, 2,
                                 1, 2);
    scrollAreaLayout->addWidget(tScale->getButton(), 13, 0,
                                 1, 2);
    scrollAreaLayout->addWidget(tRotate->getButton(), 13, 2,
                                 1, 2);
    scrollAreaLayout->addWidget(line[2], 14, 0, 1, 4);
    scrollAreaLayout->addWidget(toolVertex->getButton(), 15, 0,
                                 1, 2);
    scrollAreaLayout->addWidget(toolTriangle->getButton(), 15, 2,
                                 1, 2);
    scrollAreaLayout->addWidget(toolPlane->getButton(), 16, 0, 1,
                                2);
    scrollAreaLayout->addWidget(toolBox->getButton(), 16, 2, 1, 2);
    scrollAreaLayout->addWidget(toolEllipse->getButton(), 17, 0, 1,
                                2);
    scrollAreaLayout->addWidget(toolCylinder->getButton(), 17, 2, 1,
                                2);
    scrollAreaLayout->addWidget(line[3], 18, 0, 1, 4);
    scrollAreaLayout->addWidget(workWithWidget, 19, 0, 1, 4);
    scrollAreaLayout->addWidget(line[4], 20, 0, 1, 4);
    scrollAreaLayout->addWidget(toolSelectWidget, 21, 0, 1, 4);
    scrollAreaLayout->addWidget(toolMoveWidget, 22, 0, 1, 4);
    scrollAreaLayout->addWidget(toolScaleWidget, 23, 0, 1, 4);
    scrollAreaLayout->addWidget(toolRotateWidget, 24, 0, 1, 4);
    scrollAreaLayout->addWidget(toolVertexWidget, 25, 0, 1, 4);
    scrollAreaLayout->addWidget(toolTriangleWidget, 26, 0, 1, 4);
    scrollAreaLayout->addWidget(toolPlaneWidget, 27, 0, 1, 4);
    scrollAreaLayout->addWidget(toolBoxWidget, 28, 0, 1, 4);
    scrollAreaLayout->addWidget(toolEllipseWidget, 23, 0, 1, 4);
    scrollAreaLayout->addWidget(toolCylinderWidget, 24, 0, 1, 4);

    scrollAreaLayout->addItem(spacer, 40, 0, 1, 4);

    scrollAreaWidget->setLayout(scrollAreaLayout);
    scrollArea->setWidget(scrollAreaWidget);

    toolMoveWidget->hide();
    toolScaleWidget->hide();
    toolRotateWidget->hide();
    toolVertexWidget->hide();
    toolTriangleWidget->hide();
    toolPlaneWidget->hide();
    toolBoxWidget->hide();
    toolEllipseWidget->hide();
    toolCylinderWidget->hide();
    //scrollarea end

    //viewports
    QWidget *viewportWidget = new QWidget;
    QGridLayout *viewportLayout = new QGridLayout;

    for(i = 0; i < 4; i++) widget[i] = new GLWidget(this);

    widget[0]->setProjection(TOP);
    widget[1]->setProjection(FRONT);
    widget[2]->setProjection(LEFT);
    widget[3]->setProjection(PERSPECTIVE);

    widgetActive = widget[3];
    setActiveWidget(widget[3]);

    for(i = 0; i < 4; i++) viewportLayout->addWidget(widget[i],
                                                     i / 2, i % 2);
    viewportWidget->setLayout(viewportLayout);
    //viewports end

    //add viewports and scrollarea to window
    centralLayout->addWidget(viewportWidget, 0, 0);
    centralLayout->addWidget(scrollArea, 0, 1);
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);

    createActions();
    createMenus();

    setWindowTitle("3d Modeler");



 //  model->load( "/path/to/model.mdl" );
}

void MainWindow::open()
{
    if( saveRequest() )
    {
        if( openFileDialog( "Open" ) )
        {
            if( !model->isEmpty ) model->clear();
            model->load( model->fileName.toStdString().c_str() );
        }
    }
}

void MainWindow::createActions()
{
    openAction = new QAction( tr( "&Open" ), this );
    openAction->setShortcut( tr( "Ctrl+O" ) );
    connect( openAction, SIGNAL( triggered() ), this,
             SLOT( open() ) );
    exitAction = new QAction( tr( "&Exit" ), this );
    exitAction->setShortcut( tr( "Ctrl+Q" ) );
    connect( exitAction, SIGNAL( triggered() ), this,
             SLOT( close() ) );
    newAction = new QAction( tr( "&New" ), this );
    newAction->setShortcut( tr( "Ctrl+N" ) );
    connect( newAction, SIGNAL( triggered() ), this,
             SLOT( newFile() ) );
    saveAction = new QAction( tr( "&Save" ), this );
    saveAction->setShortcut( tr( "Ctrl+S" ) );
    connect( saveAction, SIGNAL( triggered() ), this,
             SLOT( save() ) );
    saveAsAction = new QAction( tr( "&Save As" ), this );
    connect( saveAsAction, SIGNAL( triggered() ), this,
             SLOT( saveAs() ) );

    selectAllAction = new QAction( tr( "&Select All" ), this );
    selectAllAction->setShortcut( tr( "Ctrl+A" ) );
    connect( selectAllAction, SIGNAL( triggered() ), this,
            SLOT( selectAll() ) );
    selectNoneAction = new QAction( tr( "&Select None" ), this );
    selectNoneAction->setShortcut( tr( "Ctrl+D" ) );
    connect( selectNoneAction, SIGNAL( triggered() ), this,
            SLOT( selectNone() ) );
    snapTogetherAction = new QAction( tr( "&Snap Together" ), this );
    snapTogetherAction->setShortcut( tr( "Ctrl+T" ) );
    connect( snapTogetherAction, SIGNAL( triggered() ), this,
             SLOT( snapTogether() ) );
    deleteAction = new QAction( tr( "&Delete" ), this );
    deleteAction->setShortcut( tr( "Del" ) );
    connect( deleteAction, SIGNAL( triggered() ), this,
             SLOT( deleteSlot() ) );
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu( tr( "&File" ) );
    fileMenu->addAction( newAction );
    fileMenu->addAction( openAction );
    fileMenu->addAction( saveAction );
    fileMenu->addAction( saveAsAction );
    fileMenu->addAction( exitAction );

    editMenu = menuBar()->addMenu( tr( "&Edit" ) );
    editMenu->addAction( selectAllAction );
    editMenu->addAction( selectNoneAction );
    editMenu->addAction( snapTogetherAction );
    editMenu->addAction( deleteAction );
}

void MainWindow::setActiveWidget(GLWidget *widget)
{
    widgetActive->setActive(false);
    widgetActive = widget;
    widgetActive->setActive(true);
    renderingMode->setCurrentIndex(widgetActive->
                                   getRenderingMode());
    wireframeOverlay->setChecked(widgetActive->
                                 getWireframeOverlay());
    projection->setCurrentIndex(widgetActive->getProjection());
}

void MainWindow::setActiveTool(Tool *tool)
{

    //cerr << tool->getButton()->text().toStdString();
    toolActive->setActive(false);
    toolActive = tool;
    tool->setActive(true);

}

bool MainWindow::saveRequest()
{
    if( !model->isEmpty )
    {
        if( model->isModified )
        {
            QMessageBox saveRequestMessage;
            saveRequestMessage.setText( "The model has been modified." );
            saveRequestMessage.setInformativeText(
                        "Do you want to save your changes?" );
            saveRequestMessage.setStandardButtons( QMessageBox::Save |
                        QMessageBox::Discard | QMessageBox::Cancel );
            saveRequestMessage.setDefaultButton( QMessageBox::Save );
            int result = saveRequestMessage.exec();
            switch( result )
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
    if( !model->loaded )
    {
        if( openFileDialog( "Save" ) )
            model->save();
    }
    else model->save();
}

void MainWindow::selectAll()
{
    if( workWithElements->getRadioButton( 0 )->isChecked() )
    {
        for( int i = 0; i < model->vertexNumber; i++ )
            model->vertex[ i ].setSelected( true );
    }
    else for( int i = 0; i < model->triangleNumber; i++ )
        model->triangle[ i ].setSelected( true );
}

void MainWindow::selectNone()
{
    if( workWithElements->getRadioButton( 0 )->isChecked() )
    {
        for( int i = 0; i < model->vertexNumber; i++ )
            model->vertex[ i ].setSelected( false );
    }
    else for( int i = 0; i < model->triangleNumber; i++ )
        model->triangle[ i ].setSelected( false );
}

bool MainWindow::openFileDialog( QString action )
{
    QFileDialog dialog;
    QStringList filters;
    filters << "3dModeller (*.mdl)" << "All Files (*)";
    dialog.setNameFilters( filters );
    dialog.setWindowTitle( action + " Model" );
    dialog.setLabelText( QFileDialog::Accept, action );
    bool result = dialog.exec();
    if( result )
    {
        dialog.selectedFiles();
        model->fileName = dialog.selectedFiles().join( "" );
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
    tPan->function(START, 0, 0);
}

void MainWindow::handleSelectClick( MyCheckBox *myCheckBox )
{
    if( tSelect->getElements()->getMyCheckBox( 0 )->
        isChecked() && tSelect->getElements()->
                      getMyCheckBox( 1 )->isChecked() )
    {
        for( int i = 0; i < 2; i++ ) tSelect->getElements()->
                getMyCheckBox( i )->setChecked( false );
        myCheckBox->setChecked( true );
    }
}

void MainWindow::hideViewport(int index)
{
    widget[index]->setHidden(!widget[index]->isHidden());
    bool threeViewportsHidden = true;
    for(int i = 0; i < 4; i++) if(widget[i] != widgetActive)
       threeViewportsHidden *= hideViewportButtons[i]->isChecked();
    maximizeButton->setChecked(threeViewportsHidden &&
                               !widgetActive->isHidden());
}

void MainWindow::maximize(bool value)
{
    if(value)
    {
        for(int i = 0; i < 4; i++)
        {
            widget[i]->setOldHidden(widget[i]->isHidden());
            hideViewportButtons[i]->setChecked(!(widget[i] ==
                                                 widgetActive));
        }
    }
    else for(int i = 0; i < 4; i++) hideViewportButtons[i]->
            setChecked(widget[i]->oldHidden());
}

void MainWindow::snapTogether()
{
    int i;
    vector <int> selected;
    QVector3D min, max;
    for( i = 0; i < model->vertexNumber; i++ )
    {
        if( model->vertex[ i ].isSelected() )
        {
            min = max = model->vertex[ i ].getPosition();
            selected.push_back( i );
            break;
        }
    }
    for( ; i < model->vertexNumber; i++ )
    {
        if( model->vertex[ i ].isSelected() )
        {
            selected.push_back( i );
            QVector3D vertex = model->vertex[ i ].getPosition();

            if( vertex.x() > max.x() ) max.setX( vertex.x() );
            if( vertex.y() > max.y() ) max.setY( vertex.y() );
            if( vertex.z() > max.z() ) max.setZ( vertex.z() );

            if( vertex.x() < min.x() ) min.setX( vertex.x() );
            if( vertex.y() < min.y() ) min.setY( vertex.y() );
            if( vertex.z() < min.z() ) min.setZ( vertex.z() );
        }
    }
    QVector3D center = ( max + min ) / 2;
    for( i = 0; i < selected.size(); i++ ) model->vertex[
            selected[ i ] ].setPosition( center );
}

void MainWindow::deleteSlot()
{
/*    int i, j;
    vector <int> deleteList;
    for( i = 0; i < model->triangleNumber; i++ )
    {
        if( model->triangle[ i ].isSelected() )
            deleteList.push_back( i );
    }
    int offset = 0;
    for( i = 0; i + offset < model->triangleNumber; i++ )
    {
        if( i == deleteList[ offset ] )
        {
            offset++;
            for( j = 0; i + 1 + j < model->triangleNumber; j++ )
                if( offset + j < deleteList.size() && i + 1 + j != deleteList[ offset + j ] ) break;
            offset += j;
        }
        if( offset ) model->triangle[ i ] = model->triangle[ i + offset ];
    }
    model->triangleNumber -= deleteList.size();*/

 /*   int i, j;
    vector <int> deleteList;
    for( i = 0; i < model->vertexNumber; i++ )
    {
        if( model->vertex[ i ].isSelected() )
            deleteList.push_back( i );
    }
    int offset = 0;
    for( i = 0; i + offset < model->vertexNumber; i++ )
    {
        if( i == deleteList[ offset ] )
        {
            offset++;
            for( j = 0; i + 1 + j < model->vertexNumber; j++ )
                if( offset + j < deleteList.size() && i + 1 + j != deleteList[ offset + j ] ) break;
            offset += j;
        }
        if( offset ) model->vertex[ i ] = model->vertex[ i + offset ];
    }
    model->vertexNumber -= deleteList.size();*/
}

Model *MainWindow::getModel()
{
    return model;
}

WidgetElements *MainWindow::getWorkWithElements()
{
    return workWithElements;
}

void MainWindow::stopQuickAccess()
{
    setActiveTool(lastTool);
};
