#include "recordwith1element.h"
#include "model.h"

void RecordWith1Element::updateNormals()
{
    vector<int> ver;
    for(int i = 0; i < _triangle.size(); i++) for(int j = 0; j < 3; j++) {
        int index = _triangle[i].iterator->getIndex(j);
        int k;
        for(k = 0; k < ver.size(); k++) if(ver[k] == index) break;
        if(k == ver.size()) ver.push_back(index);
    }
    for(int i = 0; i < ver.size(); i++) Model::vertex()[ver[i]].countNormals();
}
