#pragma once
#include "../Rendering/OpenGL/Camera.hpp"

#include <stdint.h>
#include <glm/vec3.hpp>

namespace Game
{
	class Player
	{
	public:
		Player(const glm::vec3& position = { 0, 0, 0 }, const glm::vec3& rotation = { 0, 0, 0 });
		~Player();

		void on_update(const uint64_t delta);

		const glm::mat4& get_view_matrix() { return camera.get_view_matrix(); };
		const glm::mat4& get_projection_matrix() const { return camera.get_projection_matrix(); }

		const glm::vec3& get_position() const { return m_position; }
		const glm::vec3& get_rotation() const { return m_rotation; }
		const glm::vec3& get_direction() const { return m_direction; }

		void set_position(const glm::vec3& position);
		void set_rotation(const glm::vec3& rotation);
		void set_position_rotation(const glm::vec3& position, const glm::vec3& rotation);
		void set_projection_mode(const Rendering::Camera::ProjectionMode projection_mode);
		void set_far_clip_plane(const float far);
		void set_near_clip_plane(const float near);
		void set_viewport_size(const float width, const float height);
		void set_field_of_view(const float fov);
		void add_rotation(const glm::vec3& rotation_delta) { m_rotation += rotation_delta; camera.add_movement_and_rotation(glm::vec3(0.f), rotation_delta); };

		uint8_t get_current() const { return current; };

	private:
		Rendering::Camera camera;

		uint8_t current = 1;
		bool isFlying = true;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_direction;

		float m_speed = 1.f;
	};
}
