#include "Chunk.hpp"
#include "Voxel.hpp"
#include "../src/Lighting/LightMap.hpp"

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

namespace Rendering
{
	Chunk::Chunk(int xpos, int ypos, int zpos) : m_x(xpos), m_y(ypos), m_z(zpos)
	{
		lightMap = new Lighting::LightMap();
		voxels = new Voxel[CHUNK_VOL];

		for (int y = 0; y < CHUNK_H; y++) {
			for (int x = 0; x < CHUNK_W; x++) {
				int real_x = x + this->m_x * CHUNK_W;
				int real_y = y + this->m_y * CHUNK_H;
				float height = -glm::perlin(glm::vec3(real_x * 0.05f, real_y * 0.05f, 0.0f));
				for (int z = 0; z < CHUNK_D; z++) {
					int real_z = z + this->m_z * CHUNK_D;
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

	Chunk::~Chunk()
	{
		delete[] voxels;
		delete lightMap;
	}

	bool Chunk::isEmpty() {
		int id = -1;
		for (int i = 0; i < CHUNK_VOL; i++) {
			if (voxels[i].id != id) {
				if (id != -1)
					return false;
				else
					id = voxels[i].id;
			}
		}
		return true;
	}
}
