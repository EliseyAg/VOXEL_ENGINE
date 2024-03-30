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
	}

	World::~World()
	{
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

        delete[] buffer;
    }

    void World::save(std::string path)
    {
        unsigned char* buffer = new unsigned char[chunks->volume * CHUNK_VOL];
        Resources::ResourceManager::writeBinaryFile(path, (const char*)buffer, chunks->volume * CHUNK_VOL);

        delete[] buffer;
    }

	void World::render(std::shared_ptr<Rendering::ShaderProgram> program)
	{
        glm::mat4 model_matrix(1.0f);
        for (size_t i = 0; i < chunks->volume; i++) {
            Rendering::Chunk* chunk = chunks->chunks[i];
            if (chunk == nullptr)
                continue;
            Rendering::Mesh* mesh = chunks->meshes[i];
            if (mesh == nullptr)
                continue;
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
