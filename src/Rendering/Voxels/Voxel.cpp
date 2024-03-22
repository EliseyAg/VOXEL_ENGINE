#include "Voxel.hpp"

namespace Rendering
{
	std::vector<std::pair<uint8_t, std::vector<std::string>>> id_subtextures = {
		std::make_pair<uint8_t, std::vector<std::string>>(0, {"Air", "Air", "Air"}),
		std::make_pair<uint8_t, std::vector<std::string>>(1, {"GrassTop", "GrassLeft", "Dirt"}),
		std::make_pair<uint8_t, std::vector<std::string>>(2, {"Dirt", "Dirt", "Dirt"}),
		std::make_pair<uint8_t, std::vector<std::string>>(3, {"Coblestone", "Coblestone", "Coblestone"}),
		std::make_pair<uint8_t, std::vector<std::string>>(4, {"WoodTop", "WoodLeft", "WoodTop"}),
		std::make_pair<uint8_t, std::vector<std::string>>(5, {"Light", "Light", "Light"}),
		std::make_pair<uint8_t, std::vector<std::string>>(6, {"Green", "Green", "Green"}),
		std::make_pair<uint8_t, std::vector<std::string>>(7, {"Unknown", "Unknown", "Unknown"}),
		std::make_pair<uint8_t, std::vector<std::string>>(8, {"BirchTop", "BirchLeft", "BirchTop"}),
		std::make_pair<uint8_t, std::vector<std::string>>(9, {"Wood_Planks", "Wood_Planks", "Wood_Planks"}),
		std::make_pair<uint8_t, std::vector<std::string>>(10, {"Wood_Planks", "Wood_Planks", "Wood_Planks"}),
	};
}
