CONFIG += staticlib

QT += opengl

SOURCES += \
    tools/tpan.cpp \
    tools/tplane.cpp \
    tools/tbox.cpp \
    tools/tcylinder.cpp \
    tools/tmove.cpp \
    tools/torbit.cpp \
    tools/trotate.cpp \
    tools/trotatecamera.cpp \
    tools/tscale.cpp \
    tools/tselect.cpp \
    tools/ttriangle.cpp \
    tools/tvertex.cpp \
    tools/tzoom.cpp \
    tools/tellipse.cpp \
                        \
    gui/mycheckbox.cpp \
    gui/mycombobox.cpp \
    gui/mypushbutton.cpp \
    gui/myspinbox.cpp \
    gui/mycheckboxmw.cpp \
    gui/mylabel.cpp \
    gui/mypushbuttonmw.cpp \
    gui/myframe.cpp \
                        \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    tool.cpp \
    vertex.cpp \
    triangle.cpp \
    glwidget.cpp \
    camera.cpp \
    toolwithwidget.cpp \
    selectableobject.cpp \
    transformingtool.cpp \
    toolwithpivot.cpp \
    mathfunctions.cpp


HEADERS += \
    tools/tpan.h \
    tools/tbox.h \
    tools/tcylinder.h \
    tools/tmove.h \
    tools/torbit.h \
    tools/tplane.h \
    tools/trotate.h \
    tools/trotatecamera.h \
    tools/tscale.h \
    tools/tselect.h \
    tools/ttriangle.h \
    tools/tvertex.h \
    tools/tzoom.h \
    tools/tellipse.h \
                        \
    gui/mycheckbox.h \
    gui/mycombobox.h \
    gui/mypushbutton.h \
    gui/myspinbox.h \
    gui/mycheckboxmw.h \
    gui/mylabel.h \
    gui/mypushbuttonmw.h \
    gui/myhframe.h \
    gui/myframe.h \
                    \
    mainwindow.h \
    model.h \
    tool.h \
    vertex.h \
    triangle.h \
    glwidget.h \
    camera.h \
    toolwithwidget.h \
    selectableobject.h \
    transformingtool.h \
    toolwithpivot.h \
    mathfunctions.h


RESOURCES += \
    shaders.qrc


# TODO:
#
# options ( colors )
# about
# panels can detatch
# all tools should be in File, Edit...
# model info

# open popular formats
# Undo, Redo
# rewrite Model class
# grid
# basic tools: create(  box, sphere, teapot? ), extrude
# textures
# light
# select option - ignore backfacing
# selecting faces in textured
# weld vertices
# delete tool (replace vector with list? forwrd_list?)
# pivot to orbit and other tools
# items selected label
# triangles selected by clicking or drawing rectangle inside them
# backface of selected triangle has another color
# plane - option for sqauare ( hold shift ), toolBox - option for cube (hold shift)
# toolBox - capture mouse movement in whole window, not in widget
# toolElipse - create from center
# box and plane - segments
# cylinder - no cap option
# option - flip triangles
# check all matrices in glwidget
# draw points over wireframe overlay
# ortho for 3d
# ellipse and cylinder - radial segments, replace creating second cap in createWallsAndSecondCap with TEllipse::createCap ?

# select - fix (blue vertices outside rectangle) - get back newSelected - comment it!
# select -> DRAW -> triangles - add bool checked[] not to repeat vertices, find similar mistakes;
# toolWithPivot - common code for vertices and triangles



# in model->load() replace fscanf with cin
# in GLWidget::draw vertices should not repeat! - fix after textures
# axis lines should be like wireframe overlay

# loading and saving model is corrupted
# tCylinder->widget() is not hidden as it is the biggest widget

# commit: replaced & with * in functions that change argument
