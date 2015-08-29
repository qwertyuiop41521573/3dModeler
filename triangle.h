#ifndef TRIANGLE_H
#define TRIANGLE_H

class Triangle
{
public:
    Triangle( int a =-1, int b =-1, int c =-1 )
    { setIndices( a, b, c ); };

    void setSelected( bool value )
    { _isSelected = value; };

    bool isSelected()
    { return _isSelected; };

    int getIndex( int num )
    { return index[ num ]; };

    void setIndex( int num, int newIndex )
    { index[ num ] = newIndex; };

    void setNewSelected( bool value )
    { _newSelected = value; };

    bool newSelected()
    { return _newSelected; };

    void setIndices( int a, int b, int c );

private:
    bool _isSelected = false;
    bool _newSelected = false;
    int index[ 3 ];
};

#endif // TRIANGLE_H
