#include "Camera.h"

using namespace glm;

Camera::Camera(float aspect, const glm::vec3& position)
	:
m_position(position)
{
	setAspect(aspect);
}

void Camera::onKey(Window::Key k, bool down)
{
	switch (k)
	{
	case Window::Key::W:
		m_upDown = down;
		break;
	case Window::Key::S:
		m_downDown = down;
		break;
	case Window::Key::A:
		m_leftDown = down;
		break;
	case Window::Key::D:
		m_rightDown = down;
		break;
	case Window::Key::UP:
		m_speed *= 2.0f;
		break;
	case Window::Key::DOWN:
		m_speed *= 0.5f;
		break;
	default:;
	}
}

void Camera::update(float dt)
{
	m_view = lookAt(m_position, m_position + m_direction, vec3(0.0f, 1.0f, 0.0f));
}
