#include "Block.hpp"

namespace Rendering
{
	Block* Block::blocks[256];

	Block::Block(unsigned int id, std::string textureTop, std::string textureLeft, std::string textureBottom)
		:id(id),
		textureFaces{ textureTop, textureLeft, textureLeft, textureLeft, textureLeft, textureBottom },
		emission{ 0, 0, 0 }
	{
	}
}
