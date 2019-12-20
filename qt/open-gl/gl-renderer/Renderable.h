#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "GLMathematics.h"

class Renderable : public GLMathematics {
protected:
  static const unsigned arrayElementsForOneVertex = 6;
  float* vertices;
  unsigned int* indices;
  unsigned int vertexArrayLength;
  unsigned int vertexCount;
  unsigned int indexArrayLength;
  unsigned int totalVertexElements;

  void createVertexArray(unsigned int size);
  void createIndexArray(unsigned int size);

  void createVerticesForPositionAndColour();

public:
  virtual ~Renderable();

  void setup(unsigned int vertexCount,
             unsigned int vertexArraySize,
             unsigned int indexArraySize);

  float* getVertices();
  unsigned int* getIndices();

  unsigned int getVertexCount();
  unsigned int getVertexArrayLength();
  unsigned int getIndexArrayLength();
  unsigned int getTotalVertexElements();

  void printVertices();
  void printIndices();

  Renderable& operator=(const Renderable& renderable) {
    for (unsigned int i = 0; i < vertexArrayLength; ++i) {
      this->vertices[i] = renderable.vertices[i];
    }

    for (unsigned int i = 0; i < indexArrayLength; ++i) {
      this->indices[i] = renderable.indices[i];
    }

    this->vertexCount = renderable.vertexCount;
    this->indexArrayLength = renderable.indexArrayLength;
    this->totalVertexElements = renderable.totalVertexElements;
    return *this;
  }

};

#endif /* RENDERABLE_H */
