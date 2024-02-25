#pragma once
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace Rendering {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& vertexArray) noexcept;
        VertexArray(VertexArray&& vertexArray) noexcept;

        void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
        void bind() const;
        void unbind() const;

    private:
        GLuint m_id = 0;
        unsigned int m_elementsCount = 0;
    };
}