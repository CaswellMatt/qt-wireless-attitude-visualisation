#include "IndexBuffer.h"
#include "Debug.h"

IndexBuffer::IndexBuffer() {

}

IndexBuffer::IndexBuffer(const unsigned int* indices, unsigned int count)
  :
    m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall( glGenBuffers(1, &m_rendererID) );
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID) );
    GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW) );
}

IndexBuffer::~IndexBuffer()
{
    GLCall( glDeleteBuffers(1, &m_rendererID) );
}

void IndexBuffer::Bind() const
{
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID) );
}

void IndexBuffer::Unbind() const
{
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}
