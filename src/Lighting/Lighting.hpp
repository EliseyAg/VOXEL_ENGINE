#pragma once
#include "LightMap.hpp"
#include "LightSolver.hpp"

namespace Rendering
{
	class Chunks;
}

namespace Lighting
{
	class Lighting
	{
	public:
		static void init(Rendering::Chunks* chunks);
		static void add(Rendering::Chunks* chunks, int x, int y, int z, unsigned int id);
		static void del(Rendering::Chunks* chunks, int x, int y, int z);
	private:
		static LightSolver* solverR;
		static LightSolver* solverG;
		static LightSolver* solverB;
		static LightSolver* solverS;
	};
}
