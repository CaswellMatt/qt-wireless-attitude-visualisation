#include "RenderableData.h"

RenderableData::RenderableData(Renderable &object)
  : vertexBuffer(object.getVertices(), object.getTotalVertexElements() * sizeof(float)),
    indexBuffer(object.getIndices(), object.getIndexArrayLength()) {

  createVerticesForPositionAndColour();

  vertexArray.AddBuffer(vertexBuffer, layout);
}

void RenderableData::createVerticesForPositionAndColour() {
  layout.AddFloat(3);
  layout.AddFloat(3);
};

RenderableData::~RenderableData() {

}
