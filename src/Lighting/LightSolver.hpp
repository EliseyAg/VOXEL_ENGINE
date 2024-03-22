#pragma once
#include <queue>

namespace Rendering
{
	class Chunks;
}

namespace Lighting
{
	struct lightentry {
		int x;
		int y;
		int z;
		unsigned char light;
	};

	class LightSolver {
		std::queue<lightentry> addqueue;
		std::queue<lightentry> remqueue;
		Rendering::Chunks* chunks;
		int channel;
	public:
		LightSolver(Rendering::Chunks* chunks, int channel);

		void add(int x, int y, int z);
		void add(int x, int y, int z, int emission);
		void remove(int x, int y, int z);
		void solve();
	};

}
