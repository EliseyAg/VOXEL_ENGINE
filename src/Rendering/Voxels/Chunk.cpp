#include "Chunk.hpp"
#include "Voxel.hpp"

#include <math.h>

namespace Rendering
{
	Chunk::Chunk(int xpos, int ypos, int zpos) : m_x(xpos), m_y(ypos), m_z(zpos)
	{
		voxels = new Voxel[CHUNK_VOL];
		for (int y = 0; y < CHUNK_H; y++) {
			for (int z = 0; z < CHUNK_D; z++) {
				for (int x = 0; x < CHUNK_W; x++) {
					int real_x = x + this->m_x * CHUNK_W;
					int real_y = y + this->m_y * CHUNK_H;
					int real_z = z + this->m_z * CHUNK_D;
					int id = real_z <= (sin(real_x * 0.3f) * 0.5f + 0.5f) * 10;
					if (real_z <= 2)
						id = 3;
					voxels[(y * CHUNK_D + z) * CHUNK_W + x].id = id;
				}
			}
		}
	}

	Chunk::~Chunk()
	{
		delete[] voxels;
	}
}
