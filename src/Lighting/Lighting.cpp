#include "Lighting.hpp"
#include "../Rendering/Voxels/Voxel.hpp"
#include "../Rendering/Voxels/Block.hpp"
#include "../Rendering/Voxels/Chunks.hpp"

namespace Lighting
{
    LightSolver* Lighting::solverR = nullptr;
    LightSolver* Lighting::solverG = nullptr;
    LightSolver* Lighting::solverB = nullptr;
    LightSolver* Lighting::solverS = nullptr;

	void Lighting::init(Rendering::Chunks* chunks)
	{
        solverR = new LightSolver(chunks, 0);
        solverG = new LightSolver(chunks, 1);
        solverB = new LightSolver(chunks, 2);
        solverS = new LightSolver(chunks, 3);

        for (int y = 0; y < chunks->m_h * CHUNK_D; y++) {
            for (int x = 0; x < chunks->m_w * CHUNK_W; x++) {
                for (int z = 0; z < chunks->m_d * CHUNK_H; z++) {
                    Rendering::Voxel* vox = chunks->get(x, y, z);
                    if (vox->id == 0) {
                        solverR->add(x, y, z, 15);
                        solverG->add(x, y, z, 15);
                        solverB->add(x, y, z, 15);
                    }
                }
            }
        }

        for (int z = 0; z < chunks->m_d * CHUNK_D; z++) {
            for (int x = 0; x < chunks->m_w * CHUNK_W; x++) {
                for (int y = chunks->m_h * CHUNK_H - 1; y >= 0; y--) {
                    Rendering::Voxel* vox = chunks->get(x, y, z);
                    Rendering::Block* block = Rendering::Block::blocks[vox->id];
                    if (block->emission[0] || block->emission[1] || block->emission[2]) {
                        break;
                    }
                    chunks->getChunkByVoxel(x, y, z)->lightMap->setS(x % CHUNK_W, y % CHUNK_H, z % CHUNK_D, 0xF);
                }
            }
        }

        for (int z = 0; z < chunks->m_d * CHUNK_D; z++) {
            for (int x = 0; x < chunks->m_w * CHUNK_W; x++) {
                for (int y = chunks->m_h * CHUNK_H - 1; y >= 0; y--) {
                    Rendering::Voxel* vox = chunks->get(x, y, z);
                    if (vox->id != 0) {
                        break;
                    }
                    if (
                        chunks->getLight(x - 1, y, z, 3) == 0 ||
                        chunks->getLight(x + 1, y, z, 3) == 0 ||
                        chunks->getLight(x, y - 1, z, 3) == 0 ||
                        chunks->getLight(x, y + 1, z, 3) == 0 ||
                        chunks->getLight(x, y, z - 1, 3) == 0 ||
                        chunks->getLight(x, y, z + 1, 3) == 0
                        ) {
                        solverS->add(x, y, z);
                    }
                    chunks->getChunkByVoxel(x, y, z)->lightMap->setS(x % CHUNK_W, y % CHUNK_H, z % CHUNK_D, 0xF);
                }
            }
        }

        solverR->solve();
        solverG->solve();
        solverB->solve();
        solverS->solve();
	}

    void Lighting::add(Rendering::Chunks* chunks, int x, int y, int z, unsigned int id)
    {
        Rendering::Block* block = Rendering::Block::blocks[id];
        solverR->remove(x, y, z);
        solverG->remove(x, y, z);
        solverB->remove(x, y, z);
        solverS->remove(x, y, z);
        for (int i = y - 1; i >= 0; i--)
        {
            solverS->remove(x, i, z);
            if (i == 0 || chunks->get(x, i - 1, z)->id != 0) {
                break;
            }
        }
        solverR->solve();
        solverG->solve();
        solverB->solve();
        solverS->solve();
        if (block->emission[0] || block->emission[1] || block->emission[2])
        {
            solverR->add(x, y, z, block->emission[0]);
            solverG->add(x, y, z, block->emission[1]);
            solverB->add(x, y, z, block->emission[2]);
            solverR->solve();
            solverG->solve();
            solverB->solve();
        }
    }

    void Lighting::del(Rendering::Chunks* chunks, int x, int y, int z)
    {
        solverR->remove(x, y, z);
        solverG->remove(x, y, z);
        solverB->remove(x, y, z);

        solverR->solve();
        solverG->solve();
        solverB->solve();

        if (chunks->getLight(x, y + 1, z, 3) == 0xF)
        {
            for (int i = y; i >= 0; i--)
            {
                if (chunks->get(x, i, z)->id != 0)
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
}
