#include "Player.hpp"
#include "../Events/Input.hpp"

namespace Game
{
	Player::Player(const glm::vec3& position, const glm::vec3& rotation)
        : m_position(position)
        , m_rotation(rotation)
	{
        camera.set_position_rotation(m_position, m_rotation);
        m_direction = camera.get_direction();
	}

	Player::~Player()
	{

	}

	void Player::on_update(const uint64_t delta)
	{
        bool move_camera = false;
        glm::vec3 movement_delta{ 0, 0, 0 };
        glm::vec3 rotation_delta{ 0, 0, 0 };

        m_direction = camera.get_direction();

        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_1))    current = 1;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_2))    current = 2;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_3))    current = 3;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_4))    current = 4;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_5))    current = 5;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_6))    current = 6;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_7))    current = 7;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_8))    current = 8;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_9))    current = 9;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_0))    current = 10;

        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_W))    movement_delta.x += m_speed * delta * 0.00000001f;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_S))    movement_delta.x -= m_speed * delta * 0.00000001f;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_D))    movement_delta.y += m_speed * delta * 0.00000001f;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_A))    movement_delta.y -= m_speed * delta * 0.00000001f;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_E))    movement_delta.z += m_speed * delta * 0.00000001f;
        if (Events::Input::IsKeyPressed(Events::KeyCode::KEY_Q))    movement_delta.z -= m_speed * delta * 0.00000001f;

        camera.add_movement_and_rotation(movement_delta, rotation_delta);
        m_position = camera.get_position();
        m_rotation = camera.get_rotation();
	}

    void Player::set_position(const glm::vec3& position)
    {
        m_position = position;
        camera.set_position(position);
    }

    void Player::set_rotation(const glm::vec3& rotation)
    {
        m_rotation = rotation;
        camera.set_rotation(rotation);
    }

    void Player::set_position_rotation(const glm::vec3& position, const glm::vec3& rotation)
    {
        m_position = position;
        m_rotation = rotation;
        camera.set_position_rotation(position, rotation);
    }

    void Player::set_projection_mode(const Rendering::Camera::ProjectionMode projection_mode)
    {
        camera.set_projection_mode(projection_mode);
    }

    void Player::set_far_clip_plane(const float far)
    {
        camera.set_far_clip_plane(far);
    }

    void Player::set_near_clip_plane(const float near)
    {
        camera.set_near_clip_plane(near);
    }

    void Player::set_viewport_size(const float width, const float height)
    {
        camera.set_viewport_size(width, height);
    }

    void Player::set_field_of_view(const float fov)
    {
        camera.set_field_of_view(fov);
    }
}
