#pragma once

#define CHUNK_W 32
#define CHUNK_H 32
#define CHUNK_D 32
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

namespace Lighting
{
	class LightMap;
}

namespace Rendering
{
	class Voxel;

	class Chunk
	{
	public:
		Voxel* voxels;
		Lighting::LightMap* lightMap;

		Chunk(int x, int y, int z);
		~Chunk();

		bool modified = true;
		int m_x, m_y, m_z;
	};
}
