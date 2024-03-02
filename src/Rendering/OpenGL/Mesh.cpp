#include "Mesh.hpp"
#include "Renderer.hpp"
#include "../src/Resources/ResourceManager.hpp"


namespace Rendering
{
    int indices[] = {
        0,   1,  2,  2,  3,  0, // front
        4,   5,  6,  6,  7,  4, // back
        8,   9, 10, 10, 11,  8, // right
        12, 13, 14, 14, 15, 12, // left
        16, 17, 18, 18, 19, 16, // top
        20, 21, 22, 22, 23, 20  // bottom
    };

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

        m_vertexCoordsBuffer->init(vertices, 3 * vert * sizeof(float));
        Rendering::VertexBufferLayout vertexCoordsLayout;
        vertexCoordsLayout.addElementLayoutFloat(3, false);
        m_vertexArray->addBuffer(*m_vertexCoordsBuffer, vertexCoordsLayout);

        m_textureCoordsBuffer->init(textureCoords, 2 * vert * sizeof(int));
        Rendering::VertexBufferLayout textureCoordsLayout;
        textureCoordsLayout.addElementLayoutFloat(2, false);
        m_vertexArray->addBuffer(*m_textureCoordsBuffer, textureCoordsLayout);

        m_indexBuffer->init(indices, vert * sizeof(int));
	}

    Mesh::~Mesh()
    {

    }

    void Mesh::render()
    {
        m_texture->bind();
        Rendering::Renderer::draw(*m_vertexArray, *m_indexBuffer, *m_shaderProgram);
        m_vertexArray->unbind();
    }
}
