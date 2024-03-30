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
		voxels = new Voxel[CHUNK_VOL];
		for (int i = 0; i < CHUNK_VOL; i++) {
			voxels[i].id = 1;
		}
		lightMap = new Lighting::LightMap();
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
