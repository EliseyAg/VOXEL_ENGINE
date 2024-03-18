#include "Voxel.hpp"

namespace Rendering
{
	std::vector<std::pair<uint8_t, std::vector<std::string>>> id_subtextures = {
		std::make_pair<uint8_t, std::vector<std::string>>(0, {"Air", "Air", "Air"}),
		std::make_pair<uint8_t, std::vector<std::string>>(1, {"GrassTop", "GrassLeft", "Dirt"}),
		std::make_pair<uint8_t, std::vector<std::string>>(2, {"Dirt", "Dirt", "Dirt"}),
		std::make_pair<uint8_t, std::vector<std::string>>(3, {"Coblestone", "Coblestone", "Coblestone"}),
		std::make_pair<uint8_t, std::vector<std::string>>(4, {"WoodTop", "WoodLeft", "WoodTop"}),
		std::make_pair<uint8_t, std::vector<std::string>>(5, {"Green", "Green", "Green"}),
	};
}
