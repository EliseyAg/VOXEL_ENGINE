#pragma once
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
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
		Mesh(size_t vert = 0, float vertices[] = {}, std::string ShaderName = "", bool is_triangle = false);
		~Mesh();

		void update(size_t vert, float vertices[], std::string ShaderName);
		void render();

	private:
		size_t m_vert = 0;
		std::shared_ptr<VertexBuffer> m_buffer;
		std::shared_ptr<VertexArray> m_vertexArray = std::make_shared<VertexArray>();
		std::shared_ptr<ShaderProgram> m_shaderProgram;
		std::shared_ptr<Texture2D> m_texture;

		bool is_Triangles = false;
	};
}
