#pragma once
#include <string>

namespace Rendering
{
	class Block
	{
	public:
		static Block* blocks[256];

		Block(unsigned int id, std::string textureTop, std::string textureLeft, std::string textureBottom);

		const unsigned int id;
		std::string textureFaces[6];
		unsigned char emission[3];
	};
}
