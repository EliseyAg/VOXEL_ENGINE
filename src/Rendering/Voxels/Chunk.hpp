#pragma once

#define CHUNK_W 32
#define CHUNK_H 32
#define CHUNK_D 32
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

namespace Rendering
{
	class Voxel;

	class Chunk
	{
	public:
		Voxel* voxels;
		Chunk();
		~Chunk();
	};
}
