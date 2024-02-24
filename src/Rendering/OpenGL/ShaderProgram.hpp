#pragma once
#include "glad/glad.h"
#include <string>

namespace Rendering
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

		bool isCompiled() const { return m_isCompiled; }
		void bind() const;

	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);

		GLuint m_ID = 0;
		bool m_isCompiled = false;
	};
}
