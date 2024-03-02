#include "Mesh.hpp"
#include "Renderer.hpp"
#include "../src/Resources/ResourceManager.hpp"


namespace Rendering
{
    int indices[] = {
                0, 1, 2, 3, 2, 1
    };

	Mesh::Mesh(size_t vert, float vertices[], std::string ShaderName) : m_vert(vert)
	{
        m_shaderProgram = Resources::ResourceManager::getShaderProgram(ShaderName);

        m_vertexCoordsBuffer->init(vertices, 3 * vert * sizeof(float));
        Rendering::VertexBufferLayout vertexCoordsLayout;
        vertexCoordsLayout.addElementLayoutFloat(3, false);
        m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);

        //m_textureCoordsBuffer.init(textureCoords, 2 * 4 * sizeof(GLfloat));
        //Rendering::VertexBufferLayout textureCoordsLayout;
        //textureCoordsLayout.addElementLayoutFloat(2, false);
        //m_vertexArray.addBuffer(m_textureCoordsBuffer, textureCoordsLayout);

        m_indexBuffer->init(indices, 6 * sizeof(int));
	}

    Mesh::~Mesh()
    {

    }

    void Mesh::render()
    {
        Rendering::Renderer::draw(*m_vertexArray, *m_indexBuffer, *m_shaderProgram);
        m_vertexArray->unbind();
    }
}
