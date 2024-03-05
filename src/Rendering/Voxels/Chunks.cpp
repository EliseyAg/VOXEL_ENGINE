#include "Chunks.hpp"
#include "Chunk.hpp"
#include "Voxel.hpp"

#include <math.h>
#include <limits.h>

namespace Rendering
{
	Chunks::Chunks(int w, int h, int d) : m_w(w), m_h(h), m_d(d) {
		volume = w * h * d;
		chunks = new Chunk * [volume];

		int index = 0;
		for (int y = 0; y < m_h; y++) {
			for (int z = 0; z < m_d; z++) {
				for (int x = 0; x < m_w; x++, index++) {
					Chunk* chunk = new Chunk(x, y, z);
					chunks[index] = chunk;
				}
			}
		}
	}

	Chunks::~Chunks() {
		for (size_t i = 0; i < volume; i++) {
			delete chunks[i];
		}
		delete[] chunks;
	}
}
