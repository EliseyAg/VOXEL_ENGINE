#include "Mesh.hpp"
#include "Renderer.hpp"
#include "../src/Resources/ResourceManager.hpp"

namespace Rendering
{
	Mesh::Mesh(size_t vert, float vertices[], std::string ShaderName, bool is_triangle) : m_vert(vert), is_Triangles(is_triangle)
	{
        update(m_vert, vertices, ShaderName);
	}

    Mesh::~Mesh()
    {

    }

    void Mesh::update(size_t vert, float vertices[], std::string ShaderName)
    {
        m_vert = vert;
        m_shaderProgram = Resources::ResourceManager::getShaderProgram(ShaderName);
        m_texture = Resources::ResourceManager::getTexture("DefaultTextureAtlas");

        if (is_Triangles)
        {
            BufferLayout buffer_layout_vec3_vec2_float
            {
                ShaderDataType::Float3,
                ShaderDataType::Float2,
                ShaderDataType::Float
            };

            m_buffer = std::make_shared<VertexBuffer>(vertices, 6 * vert * sizeof(float), buffer_layout_vec3_vec2_float);
        }
        else
        {
            BufferLayout buffer_layout_vec3_vec4
            {
                ShaderDataType::Float3,
                ShaderDataType::Float4
            };

            m_buffer = std::make_shared<VertexBuffer>(vertices, 7 * vert * sizeof(float), buffer_layout_vec3_vec4);
        }
        m_vertexArray->addBuffer(*m_buffer);
    }

    void Mesh::render()
    {
        m_texture->bind();
        Rendering::Renderer::draw(*m_vertexArray, *m_shaderProgram, m_vert, is_Triangles ? Renderer::Primitives::TRIANGLES : Renderer::Primitives::LINES);
    }
}
