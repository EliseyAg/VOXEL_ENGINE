#include "Lighting.hpp"
#include "../Rendering/Voxels/Voxel.hpp"
#include "../Rendering/Voxels/Block.hpp"
#include "../Rendering/Voxels/Chunks.hpp"

namespace Lighting
{
    Rendering::Chunks* Lighting::chunks = nullptr;
    LightSolver* Lighting::solverR = nullptr;
    LightSolver* Lighting::solverG = nullptr;
    LightSolver* Lighting::solverB = nullptr;
    LightSolver* Lighting::solverS = nullptr;

	void Lighting::init(Rendering::Chunks* chunks)
	{
        Lighting::chunks = chunks;
        solverR = new LightSolver(chunks, 0);
        solverG = new LightSolver(chunks, 1);
        solverB = new LightSolver(chunks, 2);
        solverS = new LightSolver(chunks, 3);
	}

	void Lighting::onChunkLoaded(int cx, int cy, int cz) {
		Rendering::Chunk* chunk = chunks->getChunk(cx, cy, cz);
		Rendering::Chunk* chunkUpper = chunks->getChunk(cx, cy + 1, cz);
		Rendering::Chunk* chunkLower = chunks->getChunk(cx, cy - 1, cz);
		if (chunkLower) {
			for (int z = 0; z < CHUNK_D; z++) {
				for (int x = 0; x < CHUNK_W; x++) {
					int gx = x + cx * CHUNK_W;
					int gy = cy * CHUNK_H;
					int gz = z + cz * CHUNK_D;

					int light = chunk->lightMap->getS(x, 0, z);
					int ncy = cy - 1;
					if (light < 15) {
						Rendering::Chunk* current = chunkLower;
						if (chunkLower->lightMap->getS(x, 15, z) == 0)
							continue;
						for (int y = 15;; y--) {
							if (y < 0) {
								ncy--;
								y += CHUNK_H;
							}
							if (ncy != current->m_y)
								current = chunks->getChunk(cx, ncy, cz);
							if (!current)
								break;
							Rendering::Voxel* vox = &(current->voxels[(y * CHUNK_D + z) * CHUNK_W + x]);//chunks->get(gx,gy+y,gz);
							Rendering::Block* block = Rendering::Block::blocks[vox->id];
							if (!block->lightPassing)
								break;
							//current->lightMap->setS(x,y,z, 0);
							current->modified = true;
							solverS->remove(gx, y + ncy * CHUNK_H, gz);
							current->lightMap->setS(x, y, z, 0);
						}
					}
				}
			}
		}
		if (chunkUpper) {
			for (int z = 0; z < CHUNK_D; z++) {
				for (int x = 0; x < CHUNK_W; x++) {
					int gx = x + cx * CHUNK_W;
					int gy = cy * CHUNK_H;
					int gz = z + cz * CHUNK_D;
					int ncy = cy;

					int light = chunkUpper->lightMap->getS(x, 0, z);

					Rendering::Chunk* current = chunk;
					if (light == 15) {
						for (int y = CHUNK_H - 1;; y--) {
							if (y < 0) {
								ncy--;
								y += CHUNK_H;
							}
							if (ncy != current->m_y)
								current = chunks->getChunk(cx, ncy, cz);
							if (!current)
								break;
							Rendering::Voxel* vox = &(current->voxels[(y * CHUNK_D + z) * CHUNK_W + x]);//chunks->get(gx,gy+y,gz);
							Rendering::Block* block = Rendering::Block::blocks[vox->id];
							if (!block->lightPassing)
								break;
							current->lightMap->setS(x, y, z, 15);
							current->modified = true;
							solverS->add(gx, y + ncy * CHUNK_H, gz);
						}
					}
					else if (light) {
						solverS->add(gx, gy + CHUNK_H, gz);
					}
				}
			}
		}
		else {
			for (int z = 0; z < CHUNK_D; z++) {
				for (int x = 0; x < CHUNK_W; x++) {
					int gx = x + cx * CHUNK_W;
					int gz = z + cz * CHUNK_D;
					int ncy = cy;

					Rendering::Chunk* current = chunk;
					for (int y = CHUNK_H - 1;; y--) {
						if (y < 0) {
							ncy--;
							y += CHUNK_H;
						}
						if (ncy != current->m_y)
							current = chunks->getChunk(cx, ncy, cz);
						if (!current)
							break;
						Rendering::Voxel* vox = &(current->voxels[(y * CHUNK_D + z) * CHUNK_W + x]);//chunks->get(gx,gy+y,gz);
						Rendering::Block* block = Rendering::Block::blocks[vox->id];
						if (!block->lightPassing)
							break;
						current->lightMap->setS(x, y, z, 15);
						current->modified = true;
						solverS->add(gx, y + ncy * CHUNK_H, gz);
					}
				}
			}
		}
		//std::cout << "DONE" << std::endl;
		for (unsigned int y = 0; y < CHUNK_H; y++) {
			for (unsigned int z = 0; z < CHUNK_D; z++) {
				for (unsigned int x = 0; x < CHUNK_W; x++) {
					Rendering::Voxel vox = chunk->voxels[(y * CHUNK_D + z) * CHUNK_W + x];
					Rendering::Block* block = Rendering::Block::blocks[vox.id];
					if (block->emission[0] || block->emission[1] || block->emission[2]) {
						int gx = x + cx * CHUNK_W;
						int gy = y + cy * CHUNK_H;
						int gz = z + cz * CHUNK_D;
						solverR->add(gx, gy, gz, block->emission[0]);
						solverG->add(gx, gy, gz, block->emission[1]);
						solverB->add(gx, gy, gz, block->emission[2]);
					}
				}
			}
		}
		for (int y = -1; y <= CHUNK_H; y++) {
			for (int z = -1; z <= CHUNK_D; z++) {
				for (int x = -1; x <= CHUNK_W; x++) {
					if (!(x == -1 || x == CHUNK_W || y == -1 || y == CHUNK_H || z == -1 || z == CHUNK_D))
						continue;
					int gx = x + cx * CHUNK_W;
					int gy = y + cy * CHUNK_H;
					int gz = z + cz * CHUNK_D;
					solverR->add(gx, gy, gz);
					solverG->add(gx, gy, gz);
					solverB->add(gx, gy, gz);
					solverS->add(gx, gy, gz);
				}
			}
		}

		solverR->solve();
		solverG->solve();
		solverB->solve();
		solverS->solve();

		Rendering::Chunk* other;
		other = chunks->getChunk(cx - 1, cy, cz); if (other) other->modified = true;
		other = chunks->getChunk(cx + 1, cy, cz); if (other) other->modified = true;
		other = chunks->getChunk(cx, cy - 1, cz); if (other) other->modified = true;
		other = chunks->getChunk(cx, cy + 1, cz); if (other) other->modified = true;
		other = chunks->getChunk(cx, cy, cz - 1); if (other) other->modified = true;
		other = chunks->getChunk(cx, cy, cz + 1); if (other) other->modified = true;
	}

	void Lighting::onBlockSet(int x, int y, int z, int id) {
		Rendering::Block* block = Rendering::Block::blocks[id];
		if (id == 0) {
			solverR->remove(x, y, z);
			solverG->remove(x, y, z);
			solverB->remove(x, y, z);

			solverR->solve();
			solverG->solve();
			solverB->solve();

			if (chunks->getLight(x, y + 1, z, 3) == 0xF) {
				for (int i = y; i >= 0; i--) {
					Rendering::Voxel* vox = chunks->get(x, i, z);
					if (vox == nullptr || vox->id != 0)
						break;
					solverS->add(x, i, z, 0xF);
				}
			}

			solverR->add(x, y + 1, z); solverG->add(x, y + 1, z); solverB->add(x, y + 1, z); solverS->add(x, y + 1, z);
			solverR->add(x, y - 1, z); solverG->add(x, y - 1, z); solverB->add(x, y - 1, z); solverS->add(x, y - 1, z);
			solverR->add(x + 1, y, z); solverG->add(x + 1, y, z); solverB->add(x + 1, y, z); solverS->add(x + 1, y, z);
			solverR->add(x - 1, y, z); solverG->add(x - 1, y, z); solverB->add(x - 1, y, z); solverS->add(x - 1, y, z);
			solverR->add(x, y, z + 1); solverG->add(x, y, z + 1); solverB->add(x, y, z + 1); solverS->add(x, y, z + 1);
			solverR->add(x, y, z - 1); solverG->add(x, y, z - 1); solverB->add(x, y, z - 1); solverS->add(x, y, z - 1);

			solverR->solve();
			solverG->solve();
			solverB->solve();
			solverS->solve();
		}
		else {
			solverR->remove(x, y, z);
			solverG->remove(x, y, z);
			solverB->remove(x, y, z);
			if (!block->lightPassing) {
				solverS->remove(x, y, z);
				for (int i = y - 1; i >= 0; i--) {
					solverS->remove(x, i, z);
					if (i == 0 || chunks->get(x, i - 1, z)->id != 0) {
						break;
					}
				}
				solverS->solve();
			}
			solverR->solve();
			solverG->solve();
			solverB->solve();

			if (block->emission[0] || block->emission[1] || block->emission[2]) {
				solverR->add(x, y, z, block->emission[0]);
				solverG->add(x, y, z, block->emission[1]);
				solverB->add(x, y, z, block->emission[2]);
				solverR->solve();
				solverG->solve();
				solverB->solve();
			}
		}
	}
}
