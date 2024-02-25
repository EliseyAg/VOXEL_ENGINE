#include "ResourceManager.hpp"
#include "../Rendering/OpenGL/ShaderProgram.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

namespace Resources
{
    ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;

    std::string ResourceManager::m_path;

    void ResourceManager::init(const std::string& executablePath)
    {
        size_t found = executablePath.find_last_of("/\\");
        m_path = executablePath.substr(0, found);
    }

    std::string ResourceManager::getFileString(const std::string& relativeFilePath)
    {
        std::ifstream f;
        f.open(m_path + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
        if (!f.is_open())
        {
            std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
            return std::string{};
        }

        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    std::shared_ptr<Rendering::ShaderProgram> ResourceManager::loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
    {
        std::string vertexString = getFileString(vertexPath);
        if (vertexString.empty())
        {
            std::cerr << "No vertex shader!" << std::endl;
            return nullptr;
        }

        std::string fragmentxString = getFileString(fragmentPath);
        if (fragmentxString.empty())
        {
            std::cerr << "No fragment shader!" << std::endl;
            return nullptr;
        }

        std::shared_ptr<Rendering::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Rendering::ShaderProgram>(vertexString, fragmentxString)).first->second;
        if (newShader->isCompiled())
        {
            return newShader;
        }

        std::cerr << "Can't load shader program:\n"
            << "Vertex: " << vertexPath << "\n"
            << "Fragment: " << fragmentPath << std::endl;

        return nullptr;
    }


    std::shared_ptr<Rendering::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
    {
        ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
        if (it != m_shaderPrograms.end())
        {
            return it->second;
        }
        std::cerr << "Can't find the shader program: " << shaderName << std::endl;
        return nullptr;
    }
}
