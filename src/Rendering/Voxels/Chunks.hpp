#pragma once
#include <glm/vec3.hpp>

namespace Rendering
{
	class Voxel;
	class Chunk;
	class Mesh;
	class VoxelRenderer;

	class Chunks
	{
	public:
		Chunk** chunks;
		Chunk** chunksSecond;
		Mesh** meshes;
		Mesh** meshesSecond;
		size_t volume;

		unsigned int m_w, m_h, m_d;
		int ox, oy, oz;

		Chunks(int w, int h, int d, int ox, int oy, int oz);
		~Chunks();

		Chunk* getChunkByVoxel(int x, int y, int z);
		Chunk* getChunk(int x, int y, int z);
		Voxel* get(int x, int y, int z);
		unsigned char getLight(int x, int y, int z, int channel);

		void set(int x, int y, int z, int id);
		Voxel* rayCast(glm::vec3 start, glm::vec3 dir, float maxLength, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);

		void setCenter(int x, int y, int z);
		void translate(int dx, int dy, int dz);

		bool buildMeshes(VoxelRenderer* renderer);
		bool loadVisible();
	};
}
