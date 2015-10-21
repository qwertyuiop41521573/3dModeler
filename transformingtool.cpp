#include "transformingtool.h"
#include "journal.h"

TransformingTool::TransformingTool(MainWindow *mainWindow) : ToolWithWidget(mainWindow)
{
    for(int i = 0; i < 3; i++)
    {
        spinBox[i] = new MySpinBox;
        layout->addWidget(spinBox[i], i + 1, 1);
    }
}

void TransformingTool::transform()
{
    vector <Vertex> &vertex = model->vertex();
    int i;

    if(workWithElements[0]->isChecked())
    {
        for(i = 0; i < vertex.size(); i++) if(vertex[i].exists() && vertex[i].selected()) toTransform.push_back(i);
    }
    else
    {
        vector <Triangle> &triangle = model->triangle();
        int j;

        int index;
        checked.resize(vertex.size());
        for(i = 0; i < vertex.size(); i++) checked[i] = false;

        for(i = 0; i < triangle.size(); i++)
        {
            if(triangle[i].exists() && triangle[i].selected())
            {
                for(j = 0; j < 3; j++)
                {
                    index = triangle[i].getIndex(j);
                    if(!checked[index])
                    {
                        checked[index] = true;
                        toTransform.push_back(index);
                    }
                }
            }
        }
        checked.clear();
    }
    for(i = 0; i < toTransform.size(); i++) vertex[toTransform[i]].setPosition(transformation * vertex[toTransform[i]].getPosition());
    toTransform.clear();
}

void TransformingTool::function(Action action, QMouseEvent *event)
{
    if(action == START || action == FINAL)
    {
        _busy = true;
        journal->newRecord(EDIT);

        toTransform.clear();
        vector <Vertex> &vertex = model->vertex();
        int i;

        if(workWithElements[0]->isChecked())
        {
            for(i = 0; i < vertex.size(); i++) if(vertex[i].exists() && vertex[i].selected()) toTransform.push_back(i);
        }
        else
        {
            vector <Triangle> &triangle = model->triangle();
            int j;

            int index;
            checked.resize(vertex.size());
            for(i = 0; i < vertex.size(); i++) checked[i] = false;

            for(i = 0; i < triangle.size(); i++)
            {
                if(triangle[i].exists() && triangle[i].selected())
                {
                    for(j = 0; j < 3; j++)
                    {
                        index = triangle[i].getIndex(j);
                        if(!checked[index])
                        {
                            checked[index] = true;
                            toTransform.push_back(index);
                        }
                    }
                }
            }
            checked.clear();
        }
        journal->addList(toTransform);
        if(action == FINAL) action = EXECUTE;
    }
    if(action == STOP)
    {
        _busy = false;
        journal->submit();
    }
    if(action == EXECUTE)
    {
        vector <Vertex> &vertex = model->vertex();
        for(int i = 0; i < toTransform.size(); i++) vertex[toTransform[i]].setPosition(transformation * vertex[toTransform[i]].getPosition());
        journal->transform(transformation);
    }
}
