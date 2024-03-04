#pragma once

#include <vector>
#include <glad/glad.h>

namespace Rendering {

    struct VertexBufferLayoutElement {
        GLint count;
        GLenum type;
        GLboolean normalized;
        unsigned int size;
    };

    class VertexBufferLayout {
    public:
        VertexBufferLayout();

        void reserveElements(const size_t count);
        unsigned int getStride() const { return m_stride; }
        void addElementLayoutFloat(const unsigned int count, const bool normalized);
        const std::vector<VertexBufferLayoutElement>& getLayoutElements() const { return m_layoutElements; }

    private:
        std::vector<VertexBufferLayoutElement> m_layoutElements;
        unsigned int m_stride;
    };

}