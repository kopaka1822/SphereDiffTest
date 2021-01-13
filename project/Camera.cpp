#include "Camera.h"

using namespace glm;

Camera::Camera(const glm::vec3& position)
	:
m_position(position),
m_direction(0.0, 0.0f, 1.0f)
{}

void Camera::onKey(Window::Key k, bool down)
{
	switch (k)
	{
	case Window::Key::Q:
		m_upDown = down;
		break;
	case Window::Key::E:
		m_downDown = down;
		break;
	case Window::Key::A:
		m_leftDown = down;
		break;
	case Window::Key::D:
		m_rightDown = down;
		break;
	case Window::Key::W:
		m_forwardDown = down;
		break;
	case Window::Key::S:
		m_backwardDown = down;
		break;
		
	case Window::Key::LEFT_SHIFT:
		if (down) m_speed = 10.0f;
		else m_speed = 1.0f;
		break;
	default:;
	}
}

void Camera::update(float dt)
{
	// update view for tangent space of movement
	m_view = glm::transpose(lookAt(m_position, m_position + m_direction, vec3(0.0f, 1.0f, 0.0f)));

	float speed = m_speed * m_baseSpeed * dt;
	if (m_forwardDown)
		m_position += m_direction * speed;
	if (m_backwardDown)
		m_position += -m_direction * speed;

	if (m_rightDown)
		m_position += glm::vec3(m_view[0]) * speed;
	if (m_leftDown)
		m_position += -glm::vec3(m_view[0]) * speed;

	if (m_upDown)
		m_position += -glm::vec3(m_view[1]) * speed;
	if (m_downDown)
		m_position += glm::vec3(m_view[1]) * speed;

	// final view
	m_view = glm::transpose(lookAt(m_position, m_position + m_direction, vec3(0.0f, 1.0f, 0.0f)));
}

void Camera::drag(float dx, float dy)
{
	auto speed = m_baseSpeed * m_speed * 100.0f;
	
	// rotate direction
	const auto id = glm::mat4(1.0f);
	const auto up = glm::vec3(0.0f, 1.0f, 0.0f);
	const auto side = glm::normalize(cross(m_direction, up));

	auto v = glm::rotate(id, float(dy) * speed, side) * glm::vec4(m_direction, 0.0f);
	v = glm::rotate(id, float(dx) * speed, up) * v;
	m_direction = glm::normalize(v);
}

glm::vec3 Camera::computeNonNormalizedRayDirPinhole(glm::ivec2 pixel, glm::ivec2 frameDim)
{
	// Compute sample position in screen space in [0,1] with origin at the top-left corner.
	// The camera jitter offsets the sample by +-0.5 pixels from the pixel center.
	glm::vec2 p = (glm::vec2(pixel) + glm::vec2(0.5f, 0.5f)) / glm::vec2(frameDim); // [0, 1]
	glm::vec2 ndc = glm::vec2(2, 2) * p - glm::vec2(1, 1); // [-1, 1]

	float tanHalfFovy = tan(glm::radians(45.0f) * 0.5f);
	// Compute the non-normalized ray direction assuming a pinhole camera.
	return ndc.x * m_view[0] * m_aspect * tanHalfFovy + ndc.y * m_view[1] * tanHalfFovy + m_view[2];
}
