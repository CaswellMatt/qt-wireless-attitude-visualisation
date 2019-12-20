#pragma once

#include <GL/glew.h>

#include "Debug.h"
#include "RenderableData.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

class Renderer
{
    public:
        void Clear() const;
        void Draw(const RenderableData& object, unsigned int geometryType) const;
};
