#pragma once

#include <string>
#include <memory>
#include <vector>
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

		static void setExecutablePath(const std::string& excutablePath);
		static void unloadAllResources();

		static std::shared_ptr<Rendering::ShaderProgram> loadShaderProgram(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		static std::shared_ptr<Rendering::ShaderProgram> getShaderProgram(const std::string& shaderName);

		static std::shared_ptr<Rendering::Texture2D> loadTexture(const std::string& textureName, const std::string& texturePath);
		static std::shared_ptr<Rendering::Texture2D> getTexture(const std::string& textureName);
		static std::shared_ptr<Rendering::Texture2D> loadTextureAtlas(const std::string textureName,
																	  const std::string texturePath,
																	  const std::vector<std::string> subTextures,
																	  const unsigned int subTextureWidth,
																	  const unsigned int subTextureHeight);

		static bool loadJSONResources(const std::string& JSONPath);

		static bool writeBinaryFile(std::string filename, const char* data, size_t size);
		static bool readBinaryFile(std::string filename, char* data, size_t size);

	private:
		static std::string getFileString(const std::string& relativeFilePath);

		typedef std::map<const std::string, std::shared_ptr<Rendering::ShaderProgram>> ShaderProgramsMap;
		static ShaderProgramsMap m_shaderPrograms;

		typedef std::map<const std::string, std::shared_ptr<Rendering::Texture2D>> TexturesMap;
		static TexturesMap m_textures;

		static std::string m_path;
	};
}
