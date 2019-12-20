QT       += core gui bluetooth opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -lGLEW -lGLU -lglfw

INCLUDEPATH += \
  open-gl/gl-renderer/vendor/stb_image \
  open-gl/gl-renderer/res/ \
  open-gl/gl-renderer \
  open-gl/gl-renderables \
  open-gl \
  bluetooth

RESOURCES += open-gl/gl-renderer/res/shaders/Basic.shader

SOURCES += \
    open-gl/gl-renderer/VertexBuffer.cpp \
    open-gl/gl-renderer/IndexBuffer.cpp \
    open-gl/gl-renderer/GLMathematics.cpp \
    open-gl/gl-renderer/Shader.cpp \
    open-gl/gl-renderer/VertexArray.cpp \
    open-gl/gl-renderables/Cube.cpp \
    open-gl/gl-renderables/RenderableData.cpp \
    open-gl/gl-renderables/Grid.cpp \
    open-gl/gl-renderables/Cylinder.cpp \
    open-gl/gl-renderables/Orientation.cpp \
    open-gl/gl-renderer/Renderable.cpp \
    open-gl/gl-renderer/Renderer.cpp \
    bluetooth/bleinterface.cpp \
    open-gl/glwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    open-gl/gl-renderer/VertexBuffer.h \
    open-gl/gl-renderer/GLMathematics.h \
    open-gl/gl-renderer/Shader.h \
    open-gl/gl-renderer/VertexBufferLayout.h \
    open-gl/gl-renderer/Debug.h \
    open-gl/gl-renderer/VertexArray.h \
    open-gl/gl-renderer/IndexBuffer.h \
    open-gl/gl-renderer/Renderable.h \
    open-gl/gl-renderables/Orientation.h \
    open-gl/gl-renderables/Grid.h \
    open-gl/gl-renderables/Cylinder.h \
    open-gl/gl-renderables/Centroid.h \
    open-gl/gl-renderables/Cube.h \
    open-gl/gl-renderer/Renderer.h\
    open-gl/gl-renderables/RenderableData.h \
    bluetooth/bleinterface.h \
    open-gl/glwidget.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
  open-gl/gl-renderer/res/shaders/basicshader.frag \
  open-gl/gl-renderer/res/shaders/basicshader.vert
