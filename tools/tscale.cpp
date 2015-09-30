#include "tscale.h"
#include "glwidget.h"
#include "mainwindow.h"

TScale::TScale(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Scale");
    finalButton = new QPushButton("Scale");

    elements = new WidgetElements( 3, 3, 0, 0, 1 );

    elements->getPushButton( 0 )->setText( "X" );
    elements->getPushButton( 1 )->setText( "Y" );
    elements->getPushButton( 2 )->setText( "Z" );

    for(int i = 0; i < 3; i++ )
    {
        layout->addWidget(elements->getPushButton( i ), i, 0 );
        layout->addWidget(elements->getSpinBox( i ), i, 1 );
    }

    layout->addWidget( finalButton, 3, 0, 1, 2 );
    connect(finalButton, SIGNAL( clicked() ), _mainWindow,
            SLOT( final() ) );
    _widget->hide();

}

void TScale::function(Action action, QMouseEvent *event,
                      VertexAndIndexData *data)
{
    GLWidget *widget = *_activeWidget;
    if( action == STOP ) return;
        WidgetElements *toolElements = getElements();
        Model *model = widget->getModel();
        int i, j, k;
        WidgetElements *workWithElements = widget->
                getWorkWithElements();

        if( action == START || action == FINAL )
        {
            QVector3D min, max;
            QVector3D center;
            if( workWithElements->getRadioButton( 0 )->isChecked() )
            {
                for( i = 0; i < model->vertexNumber; i++ )
                {
                    if( model->vertex[ i ].isSelected() )
                    {
                        min = max = model->vertex[ i ].getPosition();
                        break;
                    }
                }
                for( i++; i < model->vertexNumber; i++ )
                {
                    if( model->vertex[ i ].isSelected() )
                    {
                        for( j = 0; j < 3; j++ )
                        {
                            if( model->vertex[ i ].getPosition()[ j ] >
                                    max[ j ] ) max[ j ] = model->
                                    vertex[ i ].getPosition()[ j ];
                            if( model->vertex[ i ].getPosition()[ j ] <
                                    min[ j ] ) min[ j ] = model->
                                    vertex[ i ].getPosition()[ j ];
                        }
                    }
                }
            }
            else
            {
                int index;
                bool *checked = new bool[ model->vertexNumber ];
                for( i = 0; i < model->vertexNumber; i++ ) checked[ i ] = false;
                for( i = 0; i < model->triangleNumber; i++ )
                {
                    if( model->triangle[ i ].isSelected() )
                    {
                        min = max = model->vertex[ model->triangle[ i ].
                                getIndex( 0 ) ].getPosition();
                        break;
                    }
                }

                for( ; i < model->triangleNumber; i++ )
                {
                    if( model->triangle[ i ].isSelected() )
                    {
                        for( j = 0; j < 3; j++ )
                        {
                            index = model->triangle[ i ].getIndex( j );
                            if( !checked[ index ] )
                            {
                                for( k = 0; k < 3; k++ )
                                {
                                    checked[ index ] = true;
                                    if( model->vertex[ index ].getPosition()[ k ] >
                                            max[ k ] ) max[ k ] = model->
                                            vertex[ index ].getPosition()[ k ];
                                    if( model->vertex[ index ].getPosition()[ k ] <
                                            min[ k ] ) min[ k ] = model->
                                            vertex[ index ].getPosition()[ k ];
                                }
                            }
                        }
                    }
                }
            }
            center = ( min + max );
            for( i = 0; i < 3; i++ ) center[ i ] /= 2;
            widget->setPivot( center );
            widget->setPivotTransformed( widget->transform( widget->
                                                   getPivot(), true ) );
            if( action == START ) return;
        }
        QVector3D scaleFactor;
        if( action == FINAL )
        {
            for( i = 0; i < 3; i++ )
                scaleFactor[ i ] = toolElements->getSpinBox( i )->
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
            if( !perspective ) dr *= 3;

            QVector2D temp = QVector2D( widget->getStartPosition() ) -
                    widget->getPivotTransformed();

            QVector2D e[ 3 ];
            double drTransformed[ 3 ];
            for( i = 0; i < 3; i++ )
            {
                e[ i ] = widget->transform(
                            QVector3D( i == 0, i == 1, i == 2 ), false );
                drTransformed[ i ] = QVector2D::dotProduct( e[ i ], dr );
                if( QVector2D::dotProduct( e[ i ], temp ) < 0 )
                    drTransformed[ i ] *= -1;
            }

            widget->countFinalInverseMatrix( perspective );

            for( i = 0; i < 3; i++ )
                scaleFactor[ i ] = exp( drTransformed[ i ] / 50000 );
            if( action == DRAW )
            {
                data->vertices.resize( 2 );
                data->vertices[ 0 ].position = lastPosition;
                data->vertices[ 1 ].position = currentPosition;

                data->indices.resize( 2 );
                for( i = 0; i < 2; i++ ) data->indices[ i ] = i;

                for( i = 0; i < 3; i++ )
                    toolElements->getSpinBox( i )->setValue(
                                scaleFactor[ i ] );
                return;
            }
        }

        for( i = 0; i < 3; i++ ) if( toolElements->getPushButton( i )->
                                     isChecked() ) scaleFactor[ i ] = 1;

        QVector3D vector;
        QMatrix4x4 transformation;
        transformation.setToIdentity();
        transformation.translate( widget->getPivot() );
        transformation.scale( scaleFactor );
        transformation.translate(-widget->getPivot() );
        if( workWithElements->getRadioButton( 0 )->isChecked() )
        {
            for( i = 0; i < model->vertexNumber; i++ )
            {
                if( model->vertex[ i ].isSelected() )
                    model->vertex[ i ].multiplyPosition(
                                transformation );
            }
        }
        else
        {
            int index;
            bool *checked = new bool[ model->vertexNumber ];
            for( i = 0; i < model->vertexNumber;
                 i++ ) checked[ i ] = false;

            for( i = 0; i < model->triangleNumber;
                 i++ )
            {
                if( model->triangle[ i ].
                        isSelected() )
                {
                    for( j = 0; j < 3; j++ )
                    {
                        index = model->triangle[ i ].getIndex( j );
                        if( !checked[ index ] )
                        {
                            checked[ index ] = true;
                            model->vertex[ index ].multiplyPosition(
                                        transformation );
                        }
                    }
                }
            }
        }
}
