#pragma once
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "ShaderProgram.hpp"
#include "Texture2D.hpp"

#include <vector>
#include <memory>
#include <glm/vec3.hpp>

namespace Rendering
{
	class Mesh
	{
	public:
		Mesh(size_t vert, float vertices[], std::string ShaderName, std::string subTextureName);
		~Mesh();

		void render();
	private:
		size_t m_vert;
		size_t m_indices;
		std::shared_ptr<VertexBuffer> m_vertexCoordsBuffer = std::make_shared<VertexBuffer>();
		std::shared_ptr<VertexBuffer> m_textureCoordsBuffer = std::make_shared<VertexBuffer>();
		std::shared_ptr<IndexBuffer> m_indexBuffer = std::make_shared<IndexBuffer>();
		std::shared_ptr<VertexArray> m_vertexArray = std::make_shared<VertexArray>();
		std::shared_ptr<ShaderProgram> m_shaderProgram;
		std::shared_ptr<Texture2D> m_texture;
	};
}
