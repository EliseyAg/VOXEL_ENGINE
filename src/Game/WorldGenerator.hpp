#pragma once

namespace Rendering
{
	class Voxel;
}

namespace Game
{
	class WorldGenerator
	{
	public:
		static void generate(Rendering::Voxel* voxels, int x, int y, int z);
	};
}
