#include "tplane.h"
#include "glwidget.h"
#include "functions.h"
#include "mainwindow.h"
#include "gui/mylabel.h"
#include "gui/myframe.h"

TPlane::TPlane(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    button->setText("Plane");
    finalButton = new QPushButton("Create Plane");
    int i;

    checkBoxSquare = new MyCheckBoxMW;
    checkBoxSquare->setText("Square");
    layout->addWidget(checkBoxSquare, 0, 0, 1, 2);

    QLabel *center = new QLabel("Center");
    QLabel *size = new QLabel("Size");
    MyLabel *label[6];
    for(i = 0; i < 6; i++)
    {
        spinBox[i] = new MySpinBox;
        label[i] = new MyLabel(QString('X' + i % 3) + ':', 25);
        int x = 4 * ((i % 6) / 3), y = 4 * (i / 6) + i % 3 + 1;
        layout->addWidget(label[i], y, x);
        layout->addWidget(spinBox[i], y, x + 1);
    }

    layout->addWidget(center, 1, 0, 1, 2);
    layout->addWidget(size, 1, 2, 1, 2);

    for(i = 0; i < 3; i++ )
    {
        spinBox[3 + i]->setMinimum(0);
        layout->addWidget(label[i], 2 + i, 0);
        layout->addWidget(spinBox[i], 2 + i, 1);
        layout->addWidget(label[i + 3], 2 + i, 2);
        layout->addWidget(spinBox[i + 3], 2 + i, 3);
    }

    layout->addWidget(finalButton, 6, 0, 1, 4);
}

void TPlane::function(Action action, QMouseEvent *event)
{
    if(action == DRAW) return;

    GLWidget *widget = *_activeWidget;
    vector <Vertex> &vertex = model->getVertex();
    vector <Triangle> &triangle = model->getTriangle();
    int vertexSize = vertex.size();
    int triangleSize = triangle.size();
    int i;

    if(action == START)
    {
        //create the cap without other parts
        vertex.resize(vertexSize + 4);
        triangle.resize(triangleSize + 2);
        triangle[triangleSize    ].setIndices(vertexSize, vertexSize + 1,
                                              vertexSize + 2);
        triangle[triangleSize + 1].setIndices(vertexSize, vertexSize + 2,
                                              vertexSize + 3);

        QVector3D worldCoordinates = fromScreenToWorld(event, widget);
        //all cap vertices are in 1 point
        for(i = 0; i < 4; i++)
        {
            vertex[vertexSize + i] = worldCoordinates;
            vertex[vertexSize + i].setNewSelected(true);
        }
    }
    if(action == EXECUTE)
    {
            QVector2D diagonal;
            QVector3D posA, posB;
            bool square = checkBoxSquare->isChecked();
            if( widget->getProjection() == PERSPECTIVE )
            {
                double height = vertex[ vertexSize - 4 ].getPosition().z();
                QVector3D worldCoordinates = fromScreenToWorld( event, widget, true, height );
                vertex[ vertexSize - 2 ].setPosition( worldCoordinates );
                diagonal = QVector2D( vertex[ vertexSize - 2 ].getPosition() - vertex[ vertexSize - 4 ].getPosition() );
                if( square )
                {
                    QVector2D squareDiagonal( sign( diagonal.x() ), sign( diagonal.y() ) );
                    double length = QVector2D::dotProduct( diagonal, squareDiagonal ) / double( 2 );
                    diagonal = squareDiagonal * length;
                    vertex[ vertexSize - 2 ].setPosition( vertex[ vertexSize - 4 ].getPosition() + QVector3D( diagonal, 0 ) );
                }
                posA = QVector3D( vertex[ vertexSize - 4 ].getPosition().x(), vertex[ vertexSize - 2 ].getPosition().y(), height );
                posB = QVector3D( vertex[ vertexSize - 2 ].getPosition().x(), vertex[ vertexSize - 4 ].getPosition().y(), height );
            }
            else
            {
                widget->countFinalInverseMatrix( false );
                QVector2D startPosition = QVector2D( widget->getFinalMatrix() * QVector4D( vertex[ vertexSize - 4 ].getPosition(), 1 ) );
                QVector2D currentPosition = QVector2D( event->x() - widget->getHalfWidth(), widget->getHalfHeight() - event->y() );
                diagonal = currentPosition - startPosition;

                vertex[ vertexSize - 2 ].setPosition( fromScreenToWorld_vector( currentPosition, widget ) );
                diagonal = currentPosition - startPosition;
                if( square )
                {
                    QVector2D squareDiagonal( sign( diagonal.x() ), sign( diagonal.y() ) );
                    double length = QVector2D::dotProduct( diagonal, squareDiagonal ) / double( 2 );
                    diagonal = squareDiagonal * length;
                    currentPosition = startPosition + diagonal;
                }

                vertex[ vertexSize - 2 ].setPosition( fromScreenToWorld_vector( currentPosition, widget ) );
                posA = fromScreenToWorld_xy( startPosition.x(), currentPosition.y(), widget );
                posB = fromScreenToWorld_xy( currentPosition.x(), startPosition.y(), widget );
            }
            int a = ( diagonal.x() * diagonal.y() > 0 ) ? 1 : 3;
            vertex[ vertexSize - a ].setPosition( posA );
            vertex[ vertexSize - 4 + a ].setPosition( posB );
        }
        if( action == STOP )
        {
            vertexSize = vertex.size();
            if( vertex[ vertexSize - 1 ] == vertex[
                    vertexSize - 2 ] ||
                vertex[ vertexSize - 3 ] == vertex[
                    vertexSize - 2 ] )
            {
                vertex.resize( vertexSize - 4 );
                triangle.resize( triangle.size() - 2 );
            }
            else
            {
                for( int i = 1; i < 5; i++ )
                {
                    vertex[ vertexSize - i ].setNewSelected(
                                false );
                    vertex[ vertexSize - i ].setSelected( true );
                }
            }
        }
}
