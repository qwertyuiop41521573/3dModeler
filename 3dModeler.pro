CONFIG += staticlib

QT += opengl

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    tool.cpp \
    vertex.cpp \
    widgetelements.cpp \
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
    gui/mypushbuttonmw.cpp


HEADERS += \
    mainwindow.h \
    model.h \
    tool.h \
    vertex.h \
    widgetelements.h \
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
    gui/mypushbuttonmw.h


RESOURCES += \
    shaders.qrc


# TODO:
#
# move and rotate speed
# options ( colors )
# basic tools: create(  box, sphere, teapot? ), extrude
# about
# isModified
# ScrollBarAsNeeded ?
# textures
# panels can detatch
# in Model replace traingleNumber with getTriangleNumber();
# open popular formats
# light
# Undo, Redo
# select option - ignore backfacing
# fix flatShaded wireframeOverlay
# make class SelectableVector for vertex and triangle;  in toolSelectFunc
#               if (action == STOP )  -- 2 ifs can be combined
#               ( for vertices and triangles )
#
# triangles selected by clicking or drawing rectangle inside them
# backface of selected triangle has another color
# if model is wrong, viewports should display background
# read/write textured model, and non-textured too
# selecting faces in textured
# remove #include <iostream>
# cursor shapes
# unite all setters/getters in classes
# items selected label
# logs
# brush size can be changed
# create some functions in funcions.cpp
# switch(action) {} in fuctions.cpp
# weld vertices
# all tools should be in File, Edit...
# finish 'delete'
# select triangles
# when deleting, deleted item swaps with last in vector, if it is last, vector shrinks
# console
# show/hide console
# save with .mdl
# model info
# pivot to rotate camera
# messages in console for tools
# grid
# font size can be changed
# plane - check final

# plane - optin for sqauare ( hold shift )
# in functions.cpp replace "if" with "switch, case"
# class for each tool ?
# toolBox - option for cube ( hold shift )
# toolBox - capture mouse movement in whole window, now in widget
# toolElipse!! - create from center, "circle" checkBox
# toolBox - edit spinBox properties
# box and plane - segments
# tool extrude

# tool teapot
# tool sphere
# tool Cylinder!!! - FINAL ( height )
# function reverseCap for cylinder and box





# tool.cpp, h - setupButton()
# rotate around custom axis
# separate functions.h, cpp
# tool widgets - separate button width
# tPlane->widget() is not hidden as it is the biggest widget
# tool elements - remove
# remove most signals
# make cylinder : ellipse
# ellipse and cylinder: normal = (-1, 0, 0) - FIX!!
