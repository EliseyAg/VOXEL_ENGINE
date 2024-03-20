#include "LineBatch.hpp"
#include "OpenGL/Mesh.hpp"

#define LB_VERTEX_SIZE (3 + 4)

namespace Rendering
{
	LineBatch::LineBatch(size_t capacity) : capacity(capacity)
	{
		buffer = new float[capacity * LB_VERTEX_SIZE * 2];
		index = 0;
	}

	LineBatch::~LineBatch()
	{
		delete[] buffer;
	}

	void LineBatch::line(float x1, float y1, float z1, float x2, float y2, float z2, float r, float g, float b, float a) {
		if (index >= capacity * LB_VERTEX_SIZE * 2)
			return;
		buffer[index] = x1;
		buffer[index + 1] = y1;
		buffer[index + 2] = z1;
		buffer[index + 3] = r;
		buffer[index + 4] = g;
		buffer[index + 5] = b;
		buffer[index + 6] = a;
		index += LB_VERTEX_SIZE;

		buffer[index] = x2;
		buffer[index + 1] = y2;
		buffer[index + 2] = z2;
		buffer[index + 3] = r;
		buffer[index + 4] = g;
		buffer[index + 5] = b;
		buffer[index + 6] = a;
		index += LB_VERTEX_SIZE;
	}

	void LineBatch::box(float x, float y, float z, float w, float h, float d, float r, float g, float b, float a) {
		w *= 0.5f;
		h *= 0.5f;
		d *= 0.5f;

		line(x - w, y - d, z - h, x + w, y - d, z - h, r, g, b, a);
		line(x - w, y + d, z - h, x + w, y + d, z - h, r, g, b, a);
		line(x - w, y - d, z + h, x + w, y - d, z + h, r, g, b, a);
		line(x - w, y + d, z + h, x + w, y + d, z + h, r, g, b, a);

		line(x - w, y - d, z - h, x - w, y + d, z - h, r, g, b, a);
		line(x + w, y - d, z - h, x + w, y + d, z - h, r, g, b, a);
		line(x - w, y - d, z + h, x - w, y + d, z + h, r, g, b, a);
		line(x + w, y - d, z + h, x + w, y + d, z + h, r, g, b, a);

		line(x - w, y - d, z - h, x - w, y - d, z + h, r, g, b, a);
		line(x + w, y - d, z - h, x + w, y - d, z + h, r, g, b, a);
		line(x - w, y + d, z - h, x - w, y + d, z + h, r, g, b, a);
		line(x + w, y + d, z - h, x + w, y + d, z + h, r, g, b, a);
	}

	void LineBatch::render()
	{
		if (index == 0)
			return;
		Mesh* mesh = new Mesh(index / LB_VERTEX_SIZE, buffer, "LinesShaderProgram", false);
		mesh->render();
		index = 0;
		delete mesh;
	}
}
