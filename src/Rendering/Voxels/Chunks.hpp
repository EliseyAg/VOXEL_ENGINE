#pragma once
#include <glm/vec3.hpp>

namespace Rendering
{
	class Voxel;
	class Chunk;

	class Chunks
	{
	public:
		Chunk** chunks;
		size_t volume;

		unsigned int m_w, m_h, m_d;

		Chunks(int w, int h, int d);
		~Chunks();

		Chunk* getChunk(int x, int y, int z);
		Voxel* get(int x, int y, int z);
		void set(int x, int y, int z, int id);
		Voxel* rayCast(glm::vec3 start, glm::vec3 dir, float maxLength, glm::vec3& end, glm::vec3& norm, glm::vec3& iend);

		void write(unsigned char* dest);
		void read(unsigned char* source);
	};
}
