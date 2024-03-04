#include "Mesh.hpp"
#include "Renderer.hpp"
#include "../src/Resources/ResourceManager.hpp"

namespace Rendering
{
	Mesh::Mesh(size_t vert, float vertices[], std::string ShaderName, std::vector<std::string> subTexturesNames) : m_vert(vert)
	{
        m_shaderProgram = Resources::ResourceManager::getShaderProgram(ShaderName);

        m_texture = Resources::ResourceManager::getTexture("DefaultTextureAtlas");
        auto subTexture_top = m_texture->getSubTexture(std::move(subTexturesNames[0]));
        auto subTexture_bock = m_texture->getSubTexture(std::move(subTexturesNames[1]));
        auto subTexture_bottom = m_texture->getSubTexture(std::move(subTexturesNames[2]));

        BufferLayout buffer_layout_vec3_vec2
        {
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };

        m_buffer = std::make_shared<VertexBuffer>(vertices, 5 * vert * sizeof(float), buffer_layout_vec3_vec2);
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
