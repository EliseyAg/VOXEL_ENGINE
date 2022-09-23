#pragma once
#include <string>

class ShaderProgram
{
public:
	ShaderProgram(unsigned int id);
	~ShaderProgram();

	void bind();
private:
	unsigned int m_id;
};

extern ShaderProgram* load_shader(std::string vertexFile, std::string fragmentFile);
