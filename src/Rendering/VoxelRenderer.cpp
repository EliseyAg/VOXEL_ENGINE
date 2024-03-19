#include "VoxelRenderer.hpp"
#include "Voxels/Voxel.hpp"
#include "Voxels/Chunk.hpp"
#include "OpenGL/Mesh.hpp"
#include "../Resources/ResourceManager.hpp"

#define VERTEX_SIZE (3 + 2 + 0)

#define CDIV(X,A) (((X) < 0) ? ((X) / (A) - 1) : ((X) / (A)))
#define LOCAL_NEG(X, SIZE) (((X) < 0) ? ((SIZE)+(X)) : (X))
#define LOCAL(X, SIZE) ((X) >= (SIZE) ? ((X) - (SIZE)) : LOCAL_NEG(X, SIZE))
#define IS_CHUNK(X,Y,Z) (GET_CHUNK(X,Y,Z) != nullptr)
#define GET_CHUNK(X,Y,Z) (chunks[((CDIV(Y, CHUNK_H)+1) * 3 + CDIV(Z, CHUNK_D) + 1) * 3 + CDIV(X, CHUNK_W) + 1])

#define VOXEL(X,Y,Z) (GET_CHUNK(X,Y,Z)->voxels[(LOCAL(Y, CHUNK_H) * CHUNK_D + LOCAL(Z, CHUNK_D)) * CHUNK_W + LOCAL(X, CHUNK_W)])
#define IS_BLOCKED(X,Y,Z) ((!IS_CHUNK(X, Y, Z)) || (!VOXEL(X, Y, Z).isTransparent))

#define VERTEX(INDEX, X,Y,Z, U,V, L) buffer[INDEX+0] = (X);\
									buffer[INDEX+1] = (Y);\
									buffer[INDEX+2] = (Z);\
									buffer[INDEX+3] = (U);\
									buffer[INDEX+4] = (V);\
									INDEX += VERTEX_SIZE;

namespace Rendering
{
	VoxelRenderer::VoxelRenderer(size_t capacity) : capacity(capacity)
	{
		buffer = new float[capacity * VERTEX_SIZE * 6];
	}

	VoxelRenderer::~VoxelRenderer()
	{
		delete[] buffer;
	}

	Mesh* VoxelRenderer::render(Chunk* chunk, const Chunk** chunks) {
		size_t index = 0;
		for (int y = 0; y < CHUNK_H; y++) {
			for (int z = 0; z < CHUNK_D; z++) {
				for (int x = 0; x < CHUNK_W; x++) {
					Voxel vox = chunk->voxels[(y * CHUNK_D + z) * CHUNK_W + x];
					unsigned int id = vox.id;

					if (!id) {
						continue;
					}

					float l;
					float u1, u2, v1, v2;

					auto pTexture = Resources::ResourceManager::getTexture("DefaultTextureAtlas");
					auto subTexture = pTexture->getSubTexture(id_subtextures[id].second[1]);
					u1 = subTexture.leftBottomUV.x;
					u2 = subTexture.rightTopUV.x;
					v1 = subTexture.leftBottomUV.y;
					v2 = subTexture.rightTopUV.y;

					if (!IS_BLOCKED(x, y + 1, z)) {
						l = 1.0f;
						VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, l);
						VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l);

						VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u1, v2, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v1, l);
					}

					if (!IS_BLOCKED(x, y - 1, z)) {
						l = 0.75f;
						VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l);
						VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v2, l);
						VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v2, l);

						VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l);
						VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u2, v1, l);
						VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v2, l);
					}

					if (!IS_BLOCKED(x + 1, y, z)) {
						l = 0.95f;
						VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u1, v1, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u2, v1, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, l);

						VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u1, v1, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, l);
						VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u1, v2, l);
					}

					if (!IS_BLOCKED(x - 1, y, z)) {
						l = 0.85f;
						VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l);
						VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l);
						VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v1, l);

						VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u1, v1, l);
						VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v2, l);
						VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u2, v2, l);
					}

					subTexture = pTexture->getSubTexture(id_subtextures[id].second[0]);
					u1 = subTexture.leftBottomUV.x;
					u2 = subTexture.rightTopUV.x;
					v1 = subTexture.leftBottomUV.y;
					v2 = subTexture.rightTopUV.y;
					if (!IS_BLOCKED(x, y, z + 1)) {
						l = 0.9f;
						VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v1, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, l);
						VERTEX(index, x - 0.5f, y + 0.5f, z + 0.5f, u1, v2, l);

						VERTEX(index, x - 0.5f, y - 0.5f, z + 0.5f, u1, v1, l);
						VERTEX(index, x + 0.5f, y - 0.5f, z + 0.5f, u2, v1, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z + 0.5f, u2, v2, l);
					}

					subTexture = pTexture->getSubTexture(id_subtextures[id].second[2]);
					u1 = subTexture.leftBottomUV.x;
					u2 = subTexture.rightTopUV.x;
					v1 = subTexture.leftBottomUV.y;
					v2 = subTexture.rightTopUV.y;
					if (!IS_BLOCKED(x, y, z - 1)) {
						l = 0.8f;
						VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u2, v1, l);
						VERTEX(index, x - 0.5f, y + 0.5f, z - 0.5f, u2, v2, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v2, l);

						VERTEX(index, x - 0.5f, y - 0.5f, z - 0.5f, u2, v1, l);
						VERTEX(index, x + 0.5f, y + 0.5f, z - 0.5f, u1, v2, l);
						VERTEX(index, x + 0.5f, y - 0.5f, z - 0.5f, u1, v1, l);
					}
				}
			}
		}
		return new Mesh(index / VERTEX_SIZE, buffer, "DefaultShader");
	}
}
