#pragma once
#include "../Rendering/Voxels/Chunks.hpp"
#include <glm/vec3.hpp>
#include <memory>
#include <string>

namespace Rendering
{
	class Mesh;
	class Voxel;
	class VoxelRenderer;
	class ShaderProgram;
}

namespace Game
{
	class World
	{
	public:
		World(int x, int y, int z, int ox, int oy, int oz);
		~World();

		void load(std::string path);
		void save(std::string path);

		void render(std::shared_ptr<Rendering::ShaderProgram> program);

		void set(int x, int y, int z, int id);
		Rendering::Voxel* rayCast(glm::vec3 start, glm::vec3 dir, float maxLength, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);
		Rendering::Chunks* getChunks();
		size_t getChunksVolume();

		void setCenter(int x, int y, int z);
		void translate(int x, int y, int z);

		Rendering::VoxelRenderer* getRenderer() { return renderer; }

	private:
		Rendering::Chunks* chunks = nullptr;
		Rendering::Mesh** meshes;
		Rendering::VoxelRenderer* renderer;
	};
}
