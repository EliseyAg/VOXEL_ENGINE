#include "ResourceManager.hpp"
#include "../Rendering/OpenGL/ShaderProgram.hpp"
#include "../Rendering/OpenGL/Texture2D.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

namespace Resources
{
    ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
    ResourceManager::TexturesMap ResourceManager::m_textures;

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

    std::shared_ptr<Rendering::Texture2D> ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
    {
        int channels = 0;
        int width = 0;
        int height = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), &width, &height, &channels, 0);

        if (!pixels)
        {
            std::cerr << "Can't load image: " << texturePath << std::endl;
            return nullptr;
        }

        std::shared_ptr<Rendering::Texture2D> newTexture = m_textures.emplace(textureName, std::make_shared<Rendering::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE)).first->second;

        stbi_image_free(pixels);
        return newTexture;
    }

    std::shared_ptr<Rendering::Texture2D> ResourceManager::getTexture(const std::string& textureName)
    {
        TexturesMap::const_iterator it = m_textures.find(textureName);
        if (it != m_textures.end())
        {
            return it->second;
        }
        std::cerr << "Can't find the texture: " << textureName << std::endl;
        return nullptr;
    }

    std::shared_ptr<Rendering::Texture2D> ResourceManager::loadTextureAtlas(const std::string textureName,
        const std::string texturePath,
        const std::vector<std::string> subTextures,
        const unsigned int subTextureWidth,
        const unsigned int subTextureHeight)
    {
        auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
        if (pTexture)
        {
            const unsigned int textureWidth = pTexture->width();
            const unsigned int textureHeight = pTexture->height();
            unsigned int currentTextureOffsetX = 0;
            unsigned int currentTextureOffsetY = textureHeight;
            for (const auto& currentSubTextureName : subTextures)
            {
                glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
                glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);
                pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);

                currentTextureOffsetX += subTextureWidth;
                if (currentTextureOffsetX >= textureWidth)
                {
                    currentTextureOffsetX = 0;
                    currentTextureOffsetY -= subTextureHeight;
                }
            }
        }
        return pTexture;
    }
}
