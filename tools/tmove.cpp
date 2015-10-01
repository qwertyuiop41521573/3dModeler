#include "tmove.h"
#include "glwidget.h"
#include "mainwindow.h"

TMove::TMove(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Move");
    finalButton = new QPushButton("Move");

    for(int i = 0; i < 3; i++)
    {
        pushButton[i] = new MyPushButtonMW(QString('X' + i));
        spinBox[i] = new MySpinBox;
        layout->addWidget(pushButton[i], i, 0);
        layout->addWidget(spinBox[i], i, 1);
    }

    layout->addWidget(finalButton, 3, 0, 1, 2);
    connect(finalButton, SIGNAL(clicked()), _mainWindow, SLOT(
                final()));

    _widget->hide();
}

void TMove::function(Action action, QMouseEvent *event)
{
    GLWidget *widget = *_activeWidget;
    if( action == START || action == STOP ) return;
        WidgetElements *workWithElements = widget->
                getWorkWithElements();
        int i;
        QVector3D drTransformed;
        if( action == FINAL )
        {
            for( i = 0; i < 3; i++ )
                drTransformed[ i ] = spinBox[i]->
                        value();
        }
        else
        {
            bool execute = action == EXECUTE;
            QVector2D lastPosition = execute ? widget->getLastPosition() :
                                            widget->getStartPosition();
            QVector2D currentPosition = execute ? QVector2D( event->x() -
                widget->getHalfWidth(), widget->getHalfHeight() - event->
                                    y() ) : widget->getCurrentPosition();
            QVector2D dr = currentPosition - lastPosition;
            bool perspective = widget->getProjection() == PERSPECTIVE;
            if( perspective ) dr *= 10;
            widget->countFinalInverseMatrix( perspective );
            drTransformed = QVector3D( widget->getFinalInverseMatrix() *
                                 QVector4D( dr.x(), dr.y(), 0.f, 0.f ) );
            if( action == DRAW )
            {
                VertexAndIndexData *data = widget->getToolData();
                data->vertices.resize( 2 );
                data->vertices[ 0 ].position = lastPosition;
                data->vertices[ 1 ].position = currentPosition;
                data->indices.resize( 2 );
                for( i = 0; i < 2; i++ ) data->indices[ i ] = i;
                for( i = 0; i < 3; i++ )
                    spinBox[i]->setValue(
                                drTransformed[ i ] );
                return;
            }
        }

        for( i = 0; i < 3; i++ ) drTransformed[ i ] *=
                !pushButton[i]->isChecked();
        if( workWithElements->getRadioButton( 0 )->isChecked() )
        {
            for( i = 0; i < model->vertexNumber; i++ )
            {
                if( model->vertex[ i ].isSelected() )
                    model->vertex[ i ].addToPosition( drTransformed );
            }
        }
        else
        {
            bool *checked = new bool[ model->vertexNumber ];
            int index;
            for( i = 0; i < model->vertexNumber;
                 i++ ) checked[ i ] = false;
            int j;
            for( i = 0; i < model->triangleNumber; i++ )
            {
                if( model->triangle[ i ].isSelected() )
                {
                    for( j = 0; j < 3; j++ )
                    {
                        index = model->triangle[ i ].
                                getIndex( j );
                        if( !checked[ index ] )
                        {
                            checked[ index ] = true;
                            model->vertex[ index ].addToPosition(
                                        drTransformed );
                        }
                    }
                }
            }
        }
}
