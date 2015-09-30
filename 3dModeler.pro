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
    mycheckbox.cpp \
    glwidget.cpp \
    mypushbutton.cpp \
    camera.cpp \
    functions.cpp \
    mycombobox.cpp \
    tpan.cpp \
    tzoom.cpp \
    trotatecamera.cpp \
    torbit.cpp \
    tselect.cpp \
    toolwithwidget.cpp \
    tmove.cpp \
    tscale.cpp \
    trotate.cpp

HEADERS += \
    mainwindow.h \
    model.h \
    tool.h \
    vertex.h \
    widgetelements.h \
    triangle.h \
    mycheckbox.h \
    glwidget.h \
    mypushbutton.h \
    camera.h \
    functions.h \
    mycombobox.h \
    tpan.h \
    tzoom.h \
    trotatecamera.h \
    torbit.h \
    tselect.h \
    toolwithwidget.h \
    tmove.h \
    tscale.h \
    trotate.h

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

