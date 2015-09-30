#include "tvertex.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"

TVertex::TVertex(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Vertex");
    finalButton = new QPushButton("Create Vertex");

    elements = new WidgetElements(0, 3, 0, 0, 0);
    QLabel *toolVertexLabels[ 3 ];
    for(int i = 0; i < 3; i++ ) toolVertexLabels[ i ] = new QLabel(
                    QString( 'X' + i ) + ':' );
    for(int i = 0; i < 3; i++ )
    {
        toolVertexLabels[ i ]->setMaximumWidth( 50 );
        toolVertexLabels[ i ]->setAlignment( Qt::AlignRight );
        layout->addWidget( toolVertexLabels[ i ], i, 0 );
        layout->addWidget( elements->getSpinBox( i ), i, 1 );
    }
    layout->addWidget(finalButton, 3, 0, 1, 2 );
    connect(finalButton, SIGNAL( clicked() ), _mainWindow, SLOT(
                final() ) );
    _widget->hide();
}

void TVertex::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if( action == DRAW ) return;
        if( action == START || action == FINAL )
        {
            QVector3D newVertex;
            if( action == START ) newVertex = fromScreenToWorld( event, widget );
            else for( int i = 0; i < 3; i++ ) newVertex[ i ]
                = elements->getSpinBox( i )->
                    value();
            model->vertex.push_back( newVertex );
            model->vertex[ model->vertexNumber ].setNewSelected( true );
            model->vertexNumber++;
            if( action == FINAL ) action = STOP;
        }
        if( action == EXECUTE ) model->vertex[ model->vertexNumber - 1 ].setPosition(
                fromScreenToWorld( event, widget ) );
        if( action == STOP )
        {
            int vertexNumber = model->vertexNumber;
            model->vertex[ vertexNumber - 1 ].setNewSelected( false );
            model->vertex[ vertexNumber - 1 ].setSelected( true );
        }
}
