#pragma once

#define CHUNK_W 16
#define CHUNK_H 16
#define CHUNK_D 16
#define CHUNK_VOL CHUNK_W * CHUNK_H * CHUNK_D

class Voxel;

class Chunk
{
public:
	Voxel* voxels;

	Chunk();
	~Chunk();
};
