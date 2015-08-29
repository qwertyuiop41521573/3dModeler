#include "model.h"

#include <stdio.h>
#include <iostream>

using namespace std;

Model::Model( QTextBrowser *browser )
{
    _browser = browser;
}

bool Model::load( const char *newFileName )
{
    _browser->append( "Loading " + QString( newFileName ) );
    loaded = true;
    FILE *input = fopen( newFileName, "rt" );
    if( !input )
    {
        _browser->append( "    Failed to open file " + QString(
                                  newFileName ) );
        loaded = false;
    }
    else
    {
        int i, j;
        fscanf( input, "%i", &i );
        if( i != 0 && i != 1 )
        {
            _browser->append( "    Wrong \'textured\' value" );
            loaded = false;
        }
        else
        {
            textured = i;
            if( textured )
            {
                char tex[] = "";
                fscanf( input, "%s", tex );
                texture += tex;
            }

            fscanf( input, "%i", &triangleNumber );
            triangle.resize( triangleNumber );
            int index;
            for( i = 0; i < triangleNumber; i++ )
            {
                for( j = 0; j < 3; j++ )
                {
                    fscanf( input, "%i", &index );
                    triangle[ i ].setIndex( j, index );
                }
            }
            if( textured ) vertexNumber = 3 * triangleNumber;
            else fscanf( input, "%i", &vertexNumber );
            vertex.resize( vertexNumber );
            double x, y, z;
            for( i = 0; i < vertexNumber; i++ )
            {
                fscanf( input, "%le %le %le", &x, &y, &z );
                vertex[ i ].setPosition( x, y, z );
                if( textured )
                {
                    fscanf( input, "%le %le %le", &x, &y );
                    vertex[ i ].setUV( x, y );
                }
            }

            fclose( input );
            isEmpty = false;
            fileName = QString( newFileName );
        }
    }
    _browser->append( QString( "    " ) + ( loaded ? "Successfully loaded!" : "Failed to load model" ) );
    if( loaded ) _browser->append( QString( "    Vertices: " )  + QString::number( vertexNumber ) + QString( "    Triangles: " ) +
                                   QString::number( triangleNumber ) );
    return loaded;
}

void Model::clear()
{
    vertexNumber = triangleNumber = 0;
    vertex.clear();
    triangle.clear();
    isEmpty = true;
    isModified = loaded = false;
    _browser->append( "Cleared model" );
}

void Model::save()
{
    _browser->append( "Saving to " + fileName );
    bool saved = true;
    FILE *output = fopen( fileName.toStdString().c_str(), "wt" );
    if( !output )
    {
        _browser->append( "    Failed to open file " + fileName );
        saved = false;
    }
    else
    {
        int i, j;
        fprintf( output, "%i ", vertexNumber );
        for( i = 0; i < vertexNumber; i++ )
            fprintf( output, "%lg %lg %lg ", vertex[ i ].getPosition().
                     x(), vertex[ i ].getPosition().y(), vertex[ i ].
                     getPosition().z() );

        fprintf( output, "%i ", triangleNumber );
        for( i = 0; i < triangleNumber; i++ )
        {
            for( j = 0; j < 3; j++ )
                fprintf( output, "%i ", triangle[ i ].getIndex( j ) );
        }

        fclose( output );
        isModified = false;
    }
    _browser->append( QString( "    " ) + ( saved ? "Successfully saved!" : "Failed to save model" ) );
}


