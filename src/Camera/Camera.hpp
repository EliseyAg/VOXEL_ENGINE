#pragma once

#include <glm/glm.hpp>

class Camera {
	void updateVectors();
public:
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	glm::vec3 position;
	float fov;
	glm::mat4 rotation;
	Camera(glm::vec3 position, float fov);

	void rotate(float x, float y, float z);

	glm::mat4 getProjection();
	glm::mat4 getView();
};
