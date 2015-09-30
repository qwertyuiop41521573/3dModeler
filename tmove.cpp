#include "tmove.h"
#include "glwidget.h"

TMove::TMove(MainWindow *mainWindow,
             QWidget *widget, WidgetElements *newElements, QString
       finalButtonText, bool hasStage2) : ToolWithWidget(mainWindow,
          widget, newElements,finalButtonText, hasStage2)
{
    button->setText("Move");

}

void TMove::function(Action action, QMouseEvent *event,
                     VertexAndIndexData *data)
{
    GLWidget *widget = *_activeWidget;
    if( action == START || action == STOP ) return;
        WidgetElements *toolElements = getElements();
        WidgetElements *workWithElements = widget->
                getWorkWithElements();
        Model *model = widget->getModel();
        int i;
        QVector3D drTransformed;
        if( action == FINAL )
        {
            for( i = 0; i < 3; i++ )
                drTransformed[ i ] = toolElements->getSpinBox( i )->
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
                data->vertices.resize( 2 );
                data->vertices[ 0 ].position = lastPosition;
                data->vertices[ 1 ].position = currentPosition;
                data->indices.resize( 2 );
                for( i = 0; i < 2; i++ ) data->indices[ i ] = i;
                for( i = 0; i < 3; i++ )
                    toolElements->getSpinBox( i )->setValue(
                                drTransformed[ i ] );
                return;
            }
        }

        for( i = 0; i < 3; i++ ) drTransformed[ i ] *= !toolElements->
                getPushButton( i )->isChecked();
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
