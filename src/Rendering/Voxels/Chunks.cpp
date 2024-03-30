#include "Chunks.hpp"
#include "Chunk.hpp"
#include "Voxel.hpp"
#include "../VoxelRenderer.hpp"
#include "../../Game/WorldGenerator.hpp"
#include "../OpenGL/Mesh.hpp"
#include "../src/Lighting/LightMap.hpp"
#include "../src/Lighting/Lighting.hpp"

#include <math.h>
#include <limits.h>

namespace Rendering
{
	Chunks::Chunks(int w, int d, int h, int ox, int oy, int oz) : m_w(w), m_h(h), m_d(d), ox(ox), oy(oy), oz(oz)
	{
		volume = w * h * d;
		chunks = new Chunk * [volume];
		chunksSecond = new Chunk * [volume];

		meshes = new Mesh * [volume];
		meshesSecond = new Mesh * [volume];

		for (size_t i = 0; i < volume; i++) {
			chunks[i] = nullptr;
			meshes[i] = nullptr;
		}
	}

	Chunks::~Chunks()
	{
		for (size_t i = 0; i < volume; i++) {
			delete chunks[i];
		}
		delete[] chunks;
	}

	void Chunks::setCenter(int x, int y, int z)
	{
		int cx = x / CHUNK_W;
		int cy = y / CHUNK_H;
		int cz = z / CHUNK_D;
		cx -= ox;
		cy -= oy;
		cz -= oz;
		if (x < 0) cx--;
		if (y < 0) cy--;
		if (z < 0) cz--;
		cx -= m_w / 2;
		cy -= m_h / 2;
		cz -= m_d / 2;
		if (cx != 0 || cy != 0 || cz != 0)
			translate(cx, cy, cz);
	}

	void Chunks::translate(int dx, int dy, int dz)
	{
		for (unsigned int i = 0; i < volume; i++) {
			chunksSecond[i] = nullptr;
			meshesSecond[i] = nullptr;
		}
		for (unsigned int y = 0; y < m_h; y++) {
			for (unsigned int z = 0; z < m_d; z++) {
				for (unsigned int x = 0; x < m_w; x++) {
					Chunk* chunk = chunks[(y * m_d + z) * m_w + x];
					int nx = x - dx;
					int ny = y - dy;
					int nz = z - dz;
					if (chunk == nullptr)
						continue;
					Mesh* mesh = meshes[(y * m_d + z) * m_w + x];
					if (nx < 0 || ny < 0 || nz < 0 || nx >= m_w || ny >= m_h || nz >= m_d) {
						delete chunk;
						delete mesh;
						continue;
					}
					meshesSecond[(ny * m_d + nz) * m_w + nx] = mesh;
					chunksSecond[(ny * m_d + nz) * m_w + nx] = chunk;
				}
			}
		}
		Chunk** ctemp = chunks;
		chunks = chunksSecond;
		chunksSecond = ctemp;

		Mesh** mtemp = meshes;
		meshes = meshesSecond;
		meshesSecond = mtemp;

		ox += dx;
		oy += dy;
		oz += dz;
	}

	bool Chunks::buildMeshes(VoxelRenderer* renderer)
	{
		int nearX = 0;
		int nearY = 0;
		int nearZ = 0;
		int minDistance = 1000000000;
		for (unsigned int y = 0; y < m_d; y++) {
			for (unsigned int z = 1; z < m_h - 1; z++) {
				for (unsigned int x = 1; x < m_w - 1; x++) {
					int index = (y * m_d + z) * m_w + x;
					Chunk* chunk = chunks[index];
					if (chunk == nullptr)
						continue;
					Mesh* mesh = meshes[index];
					if (mesh != nullptr && !chunk->modified)
						continue;
					int lx = x - m_w / 2;
					int ly = y - m_h / 2;
					int lz = z - m_d / 2;
					int distance = (lx * lx + ly * ly + lz * lz);
					if (distance < minDistance) {
						minDistance = distance;
						nearX = x;
						nearY = y;
						nearZ = z;
					}
				}
			}
		}

		int index = (nearY * m_d + nearZ) * m_w + nearX;

		Chunk* closes[27];

		Chunk* chunk = chunks[index];
		if (chunk == nullptr)
			return false;
		Mesh* mesh = meshes[index];
		if (mesh == nullptr || chunk->modified) {
			if (mesh != nullptr)
				delete mesh;
			if (chunk->isEmpty()) {
				meshes[index] = nullptr;
				return false;
			}
			chunk->modified = false;
			for (int i = 0; i < 27; i++)
				closes[i] = nullptr;
			for (size_t j = 0; j < volume; j++) {
				Chunk* other = chunks[j];
				if (other == nullptr)
					continue;

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
			mesh = renderer->render(chunk, (const Chunk**)closes);
			meshes[index] = mesh;
			return true;
		}
		return false;
	}

	bool Chunks::loadVisible()
	{
		int nearX = 0;
		int nearY = 0;
		int nearZ = 0;
		int minDistance = 1000000000;
		for (unsigned int y = 0; y < m_d; y++) {
			for (unsigned int z = 1; z < m_h - 1; z++) {
				for (unsigned int x = 1; x < m_w - 1; x++) {
					int index = (y * m_d + z) * m_w + x;
					Chunk* chunk = chunks[index];
					if (chunk != nullptr)
						continue;
					int lx = x - m_w / 2;
					int ly = y - m_h / 2;
					int lz = z - m_d / 2;
					int distance = (lx * lx + ly * ly + lz * lz);
					if (distance < minDistance) {
						minDistance = distance;
						nearX = x;
						nearY = y;
						nearZ = z;
					}
				}
			}
		}

		int index = (nearY * m_d + nearZ) * m_w + nearX;
		Chunk* chunk = chunks[index];
		if (chunk != nullptr)
			return false;
		chunk = new Chunk(nearX + ox, nearY + oy, nearZ + oz);
		Game::WorldGenerator::generate(chunk->voxels, chunk->m_x, chunk->m_y, chunk->m_z);

		chunks[index] = chunk;
		Lighting::Lighting::onChunkLoaded(ox + nearX, oy + nearY, oz + nearZ);
		return true;
	}

	Voxel* Chunks::get(int x, int y, int z)
	{
		x -= ox * CHUNK_W;
		y -= oy * CHUNK_H;
		z -= oz * CHUNK_D;
		int cx = x / CHUNK_W;
		int cy = y / CHUNK_H;
		int cz = z / CHUNK_D;
		if (x < 0) cx--;
		if (y < 0) cy--;
		if (z < 0) cz--;
		if (cx < 0 || cy < 0 || cz < 0 || cx >= m_w || cy >= m_h || cz >= m_d)
			return nullptr;
		Chunk* chunk = chunks[(cy * m_d + cz) * m_w + cx];
		if (chunk == nullptr)
			return nullptr;
		int lx = x - cx * CHUNK_W;
		int ly = y - cy * CHUNK_H;
		int lz = z - cz * CHUNK_D;
		return &chunk->voxels[(ly * CHUNK_D + lz) * CHUNK_W + lx];
	}

	unsigned char Chunks::getLight(int x, int y, int z, int channel)
	{
		x -= ox * CHUNK_W;
		y -= oy * CHUNK_H;
		z -= oz * CHUNK_D;
		int cx = x / CHUNK_W;
		int cy = y / CHUNK_H;
		int cz = z / CHUNK_D;
		if (x < 0) cx--;
		if (y < 0) cy--;
		if (z < 0) cz--;
		if (cx < 0 || cy < 0 || cz < 0 || cx >= m_w || cy >= m_h || cz >= m_d)
			return 0;
		Chunk* chunk = chunks[(cy * m_d + cz) * m_w + cx];
		if (chunk == nullptr)
			return 0;
		int lx = x - cx * CHUNK_W;
		int ly = y - cy * CHUNK_H;
		int lz = z - cz * CHUNK_D;
		return chunk->lightMap->get(lx, ly, lz, channel);
	}

	Chunk* Chunks::getChunkByVoxel(int x, int y, int z)
	{
		x -= ox * CHUNK_W;
		y -= oy * CHUNK_H;
		z -= oz * CHUNK_D;
		int cx = x / CHUNK_W;
		int cy = y / CHUNK_H;
		int cz = z / CHUNK_D;
		if (x < 0) cx--;
		if (y < 0) cy--;
		if (z < 0) cz--;
		if (cx < 0 || cy < 0 || cz < 0 || cx >= m_w || cy >= m_h || cz >= m_d)
			return nullptr;
		return chunks[(cy * m_d + cz) * m_w + cx];
	}

	Chunk* Chunks::getChunk(int x, int y, int z)
	{
		x -= ox;
		y -= oy;
		z -= oz;
		if (x < 0 || y < 0 || z < 0 || x >= m_w || y >= m_h || z >= m_d)
			return nullptr;
		return chunks[(y * m_d + z) * m_w + x];
	}

	void Chunks::set(int x, int y, int z, int id)
	{
		x -= ox * CHUNK_W;
		y -= oy * CHUNK_H;
		z -= oz * CHUNK_D;
		int cx = x / CHUNK_W;
		int cy = y / CHUNK_H;
		int cz = z / CHUNK_D;
		if (x < 0) cx--;
		if (y < 0) cy--;
		if (z < 0) cz--;
		if (cx < 0 || cy < 0 || cz < 0 || cx >= m_w || cy >= m_h || cz >= m_d)
			return;
		Chunk* chunk = chunks[(cy * m_d + cz) * m_w + cx];
		int lx = x - cx * CHUNK_W;
		int ly = y - cy * CHUNK_H;
		int lz = z - cz * CHUNK_D;
		chunk->voxels[(ly * CHUNK_D + lz) * CHUNK_W + lx].id = id;
		chunk->modified = true;

		if (lx == 0 && (chunk = getChunk(cx - 1, cy, cz))) chunk->modified = true;
		if (ly == 0 && (chunk = getChunk(cx, cy - 1, cz))) chunk->modified = true;
		if (lz == 0 && (chunk = getChunk(cx, cy, cz - 1))) chunk->modified = true;

		if (lx == CHUNK_W - 1 && (chunk = getChunk(cx + 1, cy, cz))) chunk->modified = true;
		if (ly == CHUNK_H - 1 && (chunk = getChunk(cx, cy + 1, cz))) chunk->modified = true;
		if (lz == CHUNK_D - 1 && (chunk = getChunk(cx, cy, cz + 1))) chunk->modified = true;
	}

	Voxel* Chunks::rayCast(glm::vec3 a, glm::vec3 dir, float maxDist, glm::vec3& end, glm::vec3& norm, glm::vec3& iend)
	{
		float px = a.x;
		float py = a.y;
		float pz = a.z;

		float dx = dir.x;
		float dy = dir.y;
		float dz = dir.z;

		float t = 0.0f;
		int ix = floor(px);
		int iy = floor(py);
		int iz = floor(pz);

		float stepx = (dx > 0.0f) ? 1.0f : -1.0f;
		float stepy = (dy > 0.0f) ? 1.0f : -1.0f;
		float stepz = (dz > 0.0f) ? 1.0f : -1.0f;

		float infinity = std::numeric_limits<float>::infinity();

		float txDelta = (dx == 0.0f) ? infinity : abs(1.0f / dx);
		float tyDelta = (dy == 0.0f) ? infinity : abs(1.0f / dy);
		float tzDelta = (dz == 0.0f) ? infinity : abs(1.0f / dz);

		float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
		float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
		float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);

		float txMax = (txDelta < infinity) ? txDelta * xdist : infinity;
		float tyMax = (tyDelta < infinity) ? tyDelta * ydist : infinity;
		float tzMax = (tzDelta < infinity) ? tzDelta * zdist : infinity;

		int steppedIndex = -1;

		while (t <= maxDist) {
			Voxel* voxel = get(ix, iy, iz);
			if (voxel == nullptr || voxel->id) {
				end.x = px + t * dx;
				end.y = py + t * dy;
				end.z = pz + t * dz;

				iend.x = ix;
				iend.y = iy;
				iend.z = iz;

				norm.x = norm.y = norm.z = 0.0f;
				if (steppedIndex == 0) norm.x = -stepx;
				if (steppedIndex == 1) norm.y = -stepy;
				if (steppedIndex == 2) norm.z = -stepz;
				return voxel;
			}
			if (txMax < tyMax) {
				if (txMax < tzMax) {
					ix += stepx;
					t = txMax;
					txMax += txDelta;
					steppedIndex = 0;
				}
				else {
					iz += stepz;
					t = tzMax;
					tzMax += tzDelta;
					steppedIndex = 2;
				}
			}
			else {
				if (tyMax < tzMax) {
					iy += stepy;
					t = tyMax;
					tyMax += tyDelta;
					steppedIndex = 1;
				}
				else {
					iz += stepz;
					t = tzMax;
					tzMax += tzDelta;
					steppedIndex = 2;
				}
			}
		}
		iend.x = ix;
		iend.y = iy;
		iend.z = iz;

		end.x = px + t * dx;
		end.y = py + t * dy;
		end.z = pz + t * dz;
		norm.x = norm.y = norm.z = 0.0f;
		return nullptr;
	}
}
