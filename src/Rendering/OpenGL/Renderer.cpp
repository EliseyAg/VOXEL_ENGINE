#include "Renderer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "ShaderProgram.hpp"

namespace Rendering
{
	void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader, Primitives primitive)
	{
		shader.bind();
		vertexArray.bind();
		indexBuffer.bind();

		glDrawElements(static_cast<size_t>(primitive), indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::draw(const VertexArray& vertexArray, const ShaderProgram& shader, int vertices, Primitives primitive)
	{
		shader.bind();
		vertexArray.bind();

		if (primitive == Primitives::LINES) glLineWidth(2.0f);
		glDrawArrays(static_cast<size_t>(primitive), 0, vertices);

		vertexArray.unbind();
	}

	void Renderer::setClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Renderer::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::setDepth(const bool enable)
	{
		if (enable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void Renderer::setViewport(unsigned int width, unsigned int height, unsigned int leftOffset, unsigned int BottomOffset)
	{
		glViewport(leftOffset, BottomOffset, width, height);
	}
}