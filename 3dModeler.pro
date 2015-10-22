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
    transformingtool.cpp \
    toolwithpivot.cpp \
    mathfunctions.cpp \
    creatingtool.cpp \
    record.cpp \
    journal.cpp \
    elementcontainer.cpp \
    element.cpp


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
    transformingtool.h \
    toolwithpivot.h \
    mathfunctions.h \
    creatingtool.h \
    record.h \
    journal.h \
    elementcontainer.h \
    element.h


RESOURCES += \
    shaders.qrc


# TODO:
#
# options (colors)
# about
# panels can detatch
# all tools should be in File, Edit...
# model info
# open popular formats
# textures
# light
# selecting faces in textured

# weld vertices
# undo, redo for snap and weld
# all changes to model should be done through container,  journal should take changes from it! no objects except container should write to journal
# try to remove newRecord and submit from creatingtool
# load, save, new - check journal

# items selected label
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
# axis lines should be like wireframe overlay
# hotkeys for WorkWithElements - 1 and 2 for vertices and triangles
# basic tools: create sphere, extrude, bend ...

# in GLWidget::draw vertices should not repeat! - fix after "textures"
# select -> DRAW -> triangles - add bool checked[] not to repeat vertices (or better solution), find similar mistakes; - fix after "triangles selected by clicking or drawing rectangle inside them"
# journal - create, remove for vertex, triangle - make some abstraction
# move add(int index) to Element ?



# textures don't work
# tCylinder->widget() is not hidden as it is the biggest widget


