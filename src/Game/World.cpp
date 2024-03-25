#include "World.hpp"
#include "../Rendering/Voxels/Chunks.hpp"
#include "../Rendering/Voxels/Chunk.hpp"
#include "../Rendering/Voxels/Voxel.hpp"
#include "../Rendering/OpenGL/Mesh.hpp"
#include "../Rendering/OpenGL/ShaderProgram.hpp"
#include "../Rendering/VoxelRenderer.hpp"
#include "../Resources/ResourceManager.hpp"

#include <glm/mat4x4.hpp>

namespace Game
{
	World::World(int x, int y, int z, int ox, int oy, int oz)
	{
		renderer = new Rendering::VoxelRenderer(1024 * 1024 * 8);
		chunks = new Rendering::Chunks(x, y, z, ox, oy, oz);
		meshes = new Rendering::Mesh * [chunks->volume];
		for (size_t i = 0; i < chunks->volume; i++)
			meshes[i] = nullptr;
	}

	World::~World()
	{
		delete[] meshes;
		delete chunks;
		delete renderer;
	}

    void World::setCenter(int x, int y, int z)
    {
        chunks->setCenter(x, y, z);
    }

    void World::translate(int x, int y, int z)
    {
        chunks->translate(x, y, z);
    }

    void World::load(std::string path)
    {
        unsigned char* buffer = new unsigned char[chunks->volume * CHUNK_VOL];
        Resources::ResourceManager::readBinaryFile(path, (char*)buffer, chunks->volume * CHUNK_VOL);
        chunks->read(buffer);

        delete[] buffer;
    }

    void World::save(std::string path)
    {
        unsigned char* buffer = new unsigned char[chunks->volume * CHUNK_VOL];
        chunks->write(buffer);
        Resources::ResourceManager::writeBinaryFile(path, (const char*)buffer, chunks->volume * CHUNK_VOL);

        delete[] buffer;
    }

	void World::render(std::shared_ptr<Rendering::ShaderProgram> program)
	{
        Rendering::Chunk* closes[27];
        for (size_t i = 0; i < chunks->volume; i++) {
            Rendering::Chunk* chunk = chunks->chunks[i];
            if (!chunk->modified)
                continue;
            chunk->modified = false;
            if (meshes[i] != nullptr)
                delete meshes[i];

            for (int i = 0; i < 27; i++)
                closes[i] = nullptr;
            for (size_t j = 0; j < chunks->volume; j++) {
                Rendering::Chunk* other = chunks->chunks[j];

                int ox = other->m_x - chunk->m_x;
                int oy = other->m_y - chunk->m_y;
                int oz = other->m_z - chunk->m_z;

                if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
                    continue;

                ox += 1;
                oy += 1;
                oz += 1;
                closes[(oy * 3 + oz) * 3 + ox] = other;
            }
            Rendering::Mesh* mesh = renderer->render(chunk, (const Rendering::Chunk**)closes);
            meshes[i] = mesh;
        }

        glm::mat4 model_matrix(1.0f);
        for (size_t i = 0; i < chunks->volume; i++) {
            Rendering::Chunk* chunk = chunks->chunks[i];
            Rendering::Mesh* mesh = meshes[i];
            model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(chunk->m_x * CHUNK_W + 0.5f, chunk->m_y * CHUNK_H + 0.5f, chunk->m_z * CHUNK_D + 0.5f));
            program->setMatrix4("model_matrix", model_matrix);
            mesh->render();
        }
	}

    size_t World::getChunksVolume() { return chunks->volume; };
    Rendering::Chunks* World::getChunks() { return chunks; };

    void World::set(int x, int y, int z, int id)
    {
        chunks->set(x, y, z, id);
    }

    Rendering::Voxel* World::rayCast(glm::vec3 start, glm::vec3 dir, float maxLength, glm::vec3& end, glm::vec3& norm, glm::vec3& iend)
    {
        return chunks->rayCast(start, dir, 10.0f, end, norm, iend);
    }
}
