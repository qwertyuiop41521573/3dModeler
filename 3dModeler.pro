CONFIG += staticlib

QT += opengl

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    tool.cpp \
    vertex.cpp \
    triangle.cpp \
    glwidget.cpp \
    camera.cpp \
    functions.cpp \
    toolwithwidget.cpp \
                    \
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
    gui/myframe.cpp


HEADERS += \
    mainwindow.h \
    model.h \
    tool.h \
    vertex.h \
    triangle.h \
    glwidget.h \
    camera.h \
    functions.h \
    toolwithwidget.h \
                  \
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
    gui/myframe.h


RESOURCES += \
    shaders.qrc


# TODO:
#
# options ( colors )
# about
# panels can detatch
# open popular formats
# Undo, Redo
# isModified
# if model is wrong, viewports should display background
# read/write textured model, and non-textured too
# all tools should be in File, Edit...
# save with .mdl
# model info
# grid
# basic tools: create(  box, sphere, teapot? ), extrude
# textures
# light
# select option - ignore backfacing
# selecting faces in textured
# weld vertices
# finish 'delete'
# when deleting, deleted item swaps with last in vector, if it is last, vector
#       shrinks
# pivot to orbit
# items selected label


# make class SelectableVector for vertex and triangle;  in toolSelectFunc
#               if (action == STOP )  -- 2 ifs can be combined
#               ( for vertices and triangles )
# unite all setters/getters in classes
# switch(action) {} in Tool::fuction()
# separate functions.h, cpp

# in Model replace traingleNumber with getTriangleNumber();
# fix flatShaded wireframeOverlay

# triangles selected by clicking or drawing rectangle inside them
# backface of selected triangle has another color
# remove #include <iostream>

# plane - option for sqauare ( hold shift ), toolBox - option for cube
#       ( hold shift )
# toolBox - capture mouse movement in whole window, not in widget
# toolElipse!! - create from center
# box and plane - segments
# cylinder - no cap option
# option - flip triangles

# in model->load() replace fscanf with cin

# tool Cylinder!!! - FINAL ( height )
# rotate around custom axis
# ellipse and cylinder: normal = (-1, 0, 0) - FIX!!


# tPlane->widget() is not hidden as it is the biggest widget
# loading and saving model is corrupted


# replace * with & !!!
# halfDiagonal can be <0 - FIX!!
# tool Triangle - FIX!!

# make cosInRadians function

# correct button width


# common in cylinder and box ?
# move, rotate and scale have much common code


# remove some variables from widget (like colors)

# check all matrices in glwidget

# in tools widgets many values must be integer!
# make triangle[i].vertex[j]

# GLWidget::draw() - textured and not have similar code

# possible fix for wireframe overlay - render each line once
