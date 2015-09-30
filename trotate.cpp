#include "trotate.h"
#include "glwidget.h"
#include "functions.h"

TRotate::TRotate(MainWindow *mainWindow,
               QWidget *widget, WidgetElements *newElements, QString
        finalButtonText, bool hasStage2) : ToolWithWidget(mainWindow,
           widget, newElements,finalButtonText, hasStage2)
{
    button->setText("Rotate");

}

void TRotate::function(Action action, QMouseEvent *event,
                       VertexAndIndexData *data)
{
    GLWidget *widget = *_activeWidget;
    if( action == STOP ) return;
        WidgetElements *toolElements = getElements();
        WidgetElements *workWithElements = widget->
                getWorkWithElements();
        Model *model = widget->getModel();
        int i, j, k;

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

        double angle;
        QMatrix4x4 rotation;
        rotation.setToIdentity();
        if( action == FINAL )
        {
            angle = toolElements->getSpinBox( 0 )->value();
            if( !getAxis( toolElements, &rotation, angle ) ) return;
        }
        else
        {
            bool execute = action == EXECUTE;
            QVector2D lastPosition = execute ? widget->getLastPosition() :
                                           widget->getStartPosition();
            QVector2D currentPosition = execute ? QVector2D( event->x() -
                widget->getHalfWidth(), widget->getHalfHeight() - event->
                                    y() ) : widget->getCurrentPosition();
            QVector2D pivotTransformed = widget->getPivotTransformed();
            QVector2D toCurrent = ( currentPosition - pivotTransformed ).
                    normalized();
            QVector2D toLast = ( lastPosition - pivotTransformed ).
                    normalized();

            angle = qAcos( QVector2D::dotProduct( toCurrent, toLast )
                           ) * 180 / M_PI;
            if( toCurrent.x() * toLast.y() > toCurrent.y() * toLast.x() )
                angle *= -1;
            if( action == DRAW )
            {
                data->vertices.resize( 3 );
                data->vertices[ 0 ].position = lastPosition;
                data->vertices[ 1 ].position = pivotTransformed;
                data->vertices[ 2 ].position = currentPosition;

                data->indices.resize( 4 );
                data->indices[ 0 ] = 0;
                data->indices[ 1 ] = 1;
                data->indices[ 2 ] = 1;
                data->indices[ 3 ] = 2;

                toolElements->getSpinBox( 0 )->setValue( angle );
                return;
            }
            if( toolElements->getMyCheckBox( 0 )->isChecked() )
            {


                widget->countRotationMatrices();
                rotation *= widget->getRotationMatrixInverse();

                rotation.rotate( angle, 0.f, 0.f, 1.f );

                rotation *= widget->getRotationMatrix();

            }
            else if( !getAxis( toolElements, &rotation, angle ) ) return;
        }

        QMatrix4x4 transformation;
        transformation.setToIdentity();
        transformation.translate( widget->getPivot() );
        transformation *= rotation;
        transformation.translate(-widget->getPivot() );
        if( workWithElements->getRadioButton( 0 )->isChecked() )
        {
            for( i = 0; i < model->vertexNumber; i++ )
            {
                if( model->vertex[ i ].isSelected() ) model->
                        vertex[ i ].multiplyPosition( transformation );
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
