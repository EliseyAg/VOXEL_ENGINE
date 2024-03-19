#pragma once
#include "glad/glad.h"

namespace Rendering
{
	class VertexArray;
	class IndexBuffer;
	class ShaderProgram;

	class Renderer
	{
	public:
		enum class Primitives
		{
			TRIANGLES = GL_TRIANGLES,
			LINES = GL_LINES
		};

		static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader, Primitives primitive);
		static void draw(const VertexArray& vertexArray, const ShaderProgram& shader, int vertices, Primitives primitive);
		static void setClearColor(float r, float g, float b, float a);
		static void clear();
		static void setDepth(const bool enable);
		static void setViewport(unsigned int width, unsigned int height, unsigned int leftOffset = 0, unsigned int BottomOffset = 0);
	};
}