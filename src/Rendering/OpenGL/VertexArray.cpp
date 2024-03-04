#include "VertexArray.hpp"

#include <glad/glad.h>

namespace Rendering
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_id);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept
    {
        m_id = vertexArray.m_id;
        vertexArray.m_id = 0;
        return *this;
    }

    VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
    {
        m_id = vertexArray.m_id;
        vertexArray.m_id = 0;
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(m_id);
    }

    void VertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::addBuffer(const VertexBuffer& vertexBuffer)
    {
        bind();
        vertexBuffer.bind();

        for (const BufferElement& current_element : vertexBuffer.get_layout().get_elements())
        {
            glEnableVertexAttribArray(m_elementsCount);
            glVertexAttribPointer(
                m_elementsCount,
                static_cast<GLint>(current_element.components_count),
                current_element.component_type,
                GL_FALSE,
                static_cast<GLsizei>(vertexBuffer.get_layout().get_stride()),
                reinterpret_cast<const void*>(current_element.offset)
            );
            ++m_elementsCount;
        }
    }
}