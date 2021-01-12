#pragma once
#include "Ray.h"
#include "../framework/Window.h"
#include "../framework/glmmath.h"

class Camera
{
public:
	Camera(const glm::vec3& position);
	void onKey(Window::Key, bool down);

	const glm::mat4& getView() const { return m_view; }
	const glm::vec3& getPosition() const { return m_position; }
	void setAspect(float aspect) { m_aspect = aspect; }
	
	void update(float dt);
	
	RayData computeRayPinhole(int x, int y, int width, int height)
	{
		RayData ray;

		// Compute the normalized ray direction assuming a pinhole camera.
		ray.origin = m_position;
		ray.direction = glm::normalize(computeNonNormalizedRayDirPinhole({x, y}, {width, height}));
		ray.t = std::numeric_limits<float>::max();
		
		return ray;
	}

	void drag(float dx, float dy);
private:
	glm::vec3 computeNonNormalizedRayDirPinhole(glm::ivec2 pixel, glm::ivec2 frameDim);

	glm::mat4 m_view;
	glm::vec3 m_direction;
	float m_speed = 1.0f;
	float m_baseSpeed = 0.01f;
	glm::vec3 m_position;
	float m_aspect = 1.0f;
	
	bool m_leftDown = false;
	bool m_rightDown = false;
	bool m_upDown = false;
	bool m_downDown = false;
	bool m_forwardDown = false;
	bool m_backwardDown = false;
};
