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
		static void terminate() { delete solverR; delete solverG; delete solverB; delete solverS; }
		static void onBlockSet(int x, int y, int z, int id);
		static Rendering::Chunks* chunks;
		static void onChunkLoaded(int cx, int cy, int cz);
	private:
		static LightSolver* solverR;
		static LightSolver* solverG;
		static LightSolver* solverB;
		static LightSolver* solverS;
	};
}
