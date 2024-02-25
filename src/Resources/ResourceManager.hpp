#pragma once

#include <string>
#include <memory>
#include <map>

namespace Rendering
{
	class ShaderProgram;
	class Texture2D;
}

namespace Resources
{
	class ResourceManager
	{
	public:
		~ResourceManager() = delete;
		ResourceManager() = delete;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;
		ResourceManager& operator=(ResourceManager&&) = delete;
		ResourceManager(ResourceManager&&) = delete;

		static void init(const std::string& excutablePath);

		static std::shared_ptr<Rendering::ShaderProgram> loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		static std::shared_ptr<Rendering::ShaderProgram> getShaderProgram(const std::string& shaderName);

		static std::shared_ptr<Rendering::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
		static std::shared_ptr<Rendering::Texture2D> getTexture(const std::string& textureName);

	private:
		static std::string getFileString(const std::string& relativeFilePath);

		typedef std::map<const std::string, std::shared_ptr<Rendering::ShaderProgram>> ShaderProgramsMap;
		static ShaderProgramsMap m_shaderPrograms;

		typedef std::map<const std::string, std::shared_ptr<Rendering::Texture2D>> TexturesMap;
		static TexturesMap m_textures;

		static std::string m_path;
	};
}
