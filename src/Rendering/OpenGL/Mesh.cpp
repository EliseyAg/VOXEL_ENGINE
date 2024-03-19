#include "Mesh.hpp"
#include "Renderer.hpp"
#include "../src/Resources/ResourceManager.hpp"

namespace Rendering
{
	Mesh::Mesh(size_t vert, float vertices[], std::string ShaderName) : m_vert(vert)
	{
        m_shaderProgram = Resources::ResourceManager::getShaderProgram(ShaderName);
        m_texture = Resources::ResourceManager::getTexture("DefaultTextureAtlas");

        BufferLayout buffer_layout_vec3_vec2_float
        {
            ShaderDataType::Float3,
            ShaderDataType::Float2,
            ShaderDataType::Float
        };

        m_buffer = std::make_shared<VertexBuffer>(vertices, 6 * vert * sizeof(float), buffer_layout_vec3_vec2_float);
        m_vertexArray->addBuffer(*m_buffer);
	}

    Mesh::~Mesh()
    {

    }

    void Mesh::render()
    {
        m_texture->bind();
        Rendering::Renderer::draw(*m_vertexArray, *m_shaderProgram, m_vert);
        m_vertexArray->unbind();
    }
}
