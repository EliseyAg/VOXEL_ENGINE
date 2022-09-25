#pragma once
#include <string>
#include <glm/glm.hpp>

class ShaderProgram
{
public:
	ShaderProgram(unsigned int id);
	~ShaderProgram();

	void bind();
	void setMat4(std::string name, glm::mat4& matrix);
private:
	unsigned int m_id;
};

extern ShaderProgram* load_shader(std::string vertexFile, std::string fragmentFile);
