#include "ShaderProgram.hpp"

#include <string>
#include <iostream>

namespace Rendering
{
    bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
    {
        shaderID = glCreateShader(shaderType);
        const char* code = source.c_str();
        glShaderSource(shaderID, 1, &code, nullptr);
        glCompileShader(shaderID);

        GLint success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile-time error:\n" << infoLog << std::endl;
            return false;
        }
        return true;
    }

    ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
        GLuint vertexShaderID;
        if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
        {
            std::cerr << "VERTEX SHADER compile-time error" << std::endl;
            glDeleteShader(vertexShaderID);
            return;
        }
        GLuint fragmentShaderID;
        if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
        {
            std::cerr << "FRAGMENT SHADER compile-time error" << std::endl;
            glDeleteShader(fragmentShaderID);
            return;
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShaderID);
        glAttachShader(m_ID, fragmentShaderID);
        glLinkProgram(m_ID);

        GLint success;
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar infoLog[1024];
            glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog << std::endl;
            return;
        }
        else
        {
            m_isCompiled = true;
        }

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
	}

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram(m_ID);
    }

    void ShaderProgram::bind() const
    {
        glUseProgram(m_ID);
    }

    ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
    {
        glDeleteProgram(m_ID);
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
    {
        m_ID = shaderProgram.m_ID;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_ID = 0;
        shaderProgram.m_isCompiled = false;
    }

    void ShaderProgram::setInt(const std::string& name, const GLint value)
    {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value)
    {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
    }


    void ShaderProgram::setMatrix4(const char* name, const glm::mat4& matrix) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }
}
