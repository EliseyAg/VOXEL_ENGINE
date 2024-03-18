#pragma once
#include <stdint.h>
#include <string>
#include <vector>

namespace Rendering
{
	extern std::vector<std::pair<uint8_t, std::vector<std::string>>> id_subtextures;

	class Voxel
	{
	public:
		uint8_t id;
		std::string name;
	};
}
