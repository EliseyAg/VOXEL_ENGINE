#pragma once
#include <stdint.h>
#include <string>
#include <vector>

namespace Rendering
{
	class Voxel
	{
	public:
		uint8_t id;
		bool isTransparent;
		std::string name;
	};
}
