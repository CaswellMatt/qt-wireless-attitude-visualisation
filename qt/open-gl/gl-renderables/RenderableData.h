#ifndef RENDERABLEDATA_H
#define RENDERABLEDATA_H

#include "Renderable.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class RenderableData {
public:
  RenderableData(Renderable &object);
  virtual ~RenderableData();

  VertexArray vertexArray;
  VertexBuffer vertexBuffer;
  IndexBuffer indexBuffer;
  VertexBufferLayout layout;

  void createVerticesForPositionAndColour();
};

#endif
