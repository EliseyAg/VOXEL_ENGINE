#pragma once
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "glad/glad.h"

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
		void unbind() const;

		void setInt(const std::string& name, const GLint value);
		void setVec3(const std::string& name, const glm::vec3& value);
		void setMatrix4(const char* name, const glm::mat4& matrix) const;

	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);

		GLuint m_ID = 0;
		bool m_isCompiled = false;
	};
}
