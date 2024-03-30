#include "WorldGenerator.hpp"
#include "../Rendering/Voxels/Voxel.hpp"
#include "../Rendering/Voxels/Chunk.hpp"

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

namespace Game
{
	void WorldGenerator::generate(Rendering::Voxel* voxels, int cx, int cy, int cz)
	{
		for (int y = 0; y < CHUNK_H; y++) {
			for (int x = 0; x < CHUNK_W; x++) {
				int real_x = x + cx * CHUNK_W;
				int real_y = y + cy * CHUNK_H;
				float height = -glm::perlin(glm::vec3(real_x * 0.05f, real_y * 0.05f, 0.0f));
				for (int z = 0; z < CHUNK_D; z++) {
					int real_z = z + cz * CHUNK_D;
					int id = real_z <= (height) * 10 + 7;
					if (real_z < height * 10 + 6)
						id = 2;
					if (real_z <= 5)
						id = 3;
					voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = id;
				}
			}
		}
	}
}
