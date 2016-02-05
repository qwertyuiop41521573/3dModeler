#include "recordwith1element.h"
#include "model.h"

void RecordWith1Element::updateNormals()
{
    VectUniqe<int> ver;
    for(int i = 0; i < _triangle.size(); i++) for(int j = 0; j < 3; j++) {
        int index = _triangle[i].iterator->getIndex(j);
        ver.push(index);
    }
    for(int i = 0; i < ver.size(); i++) Model::vertex()[ver[i]].countNormals();
}
