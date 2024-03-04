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
        const float textureCoords[] = {
            // U  V
            subTexture_bock.leftBottomUV.x, subTexture_bock.leftBottomUV.y,
            subTexture_bock.rightTopUV.x, subTexture_bock.leftBottomUV.y,
            subTexture_bock.rightTopUV.x, subTexture_bock.rightTopUV.y,
            subTexture_bock.leftBottomUV.x, subTexture_bock.rightTopUV.y,

            subTexture_bock.rightTopUV.x, subTexture_bock.leftBottomUV.y,
            subTexture_bock.leftBottomUV.x, subTexture_bock.leftBottomUV.y,
            subTexture_bock.leftBottomUV.x, subTexture_bock.rightTopUV.y,
            subTexture_bock.rightTopUV.x, subTexture_bock.rightTopUV.y,

            subTexture_bock.leftBottomUV.x, subTexture_bock.leftBottomUV.y,
            subTexture_bock.rightTopUV.x, subTexture_bock.leftBottomUV.y,
            subTexture_bock.rightTopUV.x, subTexture_bock.rightTopUV.y,
            subTexture_bock.leftBottomUV.x, subTexture_bock.rightTopUV.y,

            subTexture_bock.rightTopUV.x, subTexture_bock.leftBottomUV.y,
            subTexture_bock.leftBottomUV.x, subTexture_bock.leftBottomUV.y,
            subTexture_bock.leftBottomUV.x, subTexture_bock.rightTopUV.y,
            subTexture_bock.rightTopUV.x, subTexture_bock.rightTopUV.y,

            subTexture_top.leftBottomUV.x, subTexture_top.leftBottomUV.y,
            subTexture_top.rightTopUV.x, subTexture_top.leftBottomUV.y,
            subTexture_top.rightTopUV.x, subTexture_top.rightTopUV.y,
            subTexture_top.leftBottomUV.x, subTexture_top.rightTopUV.y,

            subTexture_bottom.leftBottomUV.x, subTexture_bottom.rightTopUV.y,
            subTexture_bottom.rightTopUV.x, subTexture_bottom.rightTopUV.y,
            subTexture_bottom.rightTopUV.x, subTexture_bottom.leftBottomUV.y,
            subTexture_bottom.leftBottomUV.x, subTexture_bottom.leftBottomUV.y,
        };

        m_buffer->init(vertices, 3 * vert * sizeof(float));
        Rendering::VertexBufferLayout vertexCoordsLayout;
        vertexCoordsLayout.addElementLayoutFloat(3, false);
        m_vertexArray->addBuffer(*m_buffer, vertexCoordsLayout);
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
