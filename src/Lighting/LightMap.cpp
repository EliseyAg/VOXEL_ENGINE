#include "LightMap.hpp"

namespace Lighting
{
	LightMap::LightMap() {
		map = new unsigned short[CHUNK_VOL];
		for (unsigned int i = 0; i < CHUNK_VOL; i++) {
			map[i] = 0x0000;
		}
	}

	LightMap::~LightMap() {
		delete[] map;
	}
}
