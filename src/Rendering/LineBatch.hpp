#pragma once
#include <memory>

namespace Rendering
{
	class Mesh;

	class LineBatch
	{
	public:
		LineBatch(size_t capacity);
		~LineBatch();

		void line(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b, float a);
		void box(float x, float y, float z, float w, float h, float d, float r, float g, float b, float a);

		void render();

	private:
		float* buffer;
		size_t capacity;
		size_t index;
	};
}
