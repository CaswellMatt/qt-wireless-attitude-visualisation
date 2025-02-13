#ifndef RENDERABLEDATA_H
#define RENDERABLEDATA_H

#include "Renderable.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class RenderableData {
public:
  RenderableData(Renderable &object);
  virtual ~RenderableData();

  QOpenGLVertexArrayObject m_vertexArray;
  QOpenGLBuffer m_vertexBuffer;
  QOpenGLBuffer m_indexBuffer;


  VertexArray vertexArray;
  VertexBuffer vertexBuffer;
  IndexBuffer indexBuffer;
  VertexBufferLayout layout;

  void createVerticesForPositionAndColour();
};

#endif
