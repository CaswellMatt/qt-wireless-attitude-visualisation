#include "Renderer.h"
#include <iostream>

void Renderer::Clear() const
{
  GLCall( glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) );
  GLCall( glClear( GL_STENCIL_BUFFER_BIT ) );
}

void Renderer::Draw(const RenderableData& data, unsigned int geometryType) const
{
  data.vertexArray.Bind();
  data.indexBuffer.Bind();

  GLCall( glDrawElements(geometryType, data.indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr) );
}

