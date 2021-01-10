#pragma once
#include "Ray.h"
#include "../framework/Window.h"
#include "../framework/glmmath.h"

class Camera
{
public:
	Camera(float aspect, const glm::vec3& position);
	void onKey(Window::Key, bool down);

	const glm::mat4& getView() const { return m_view; }
	const glm::vec3& getPosition() const { return m_position; }
	void setAspect(float aspect);

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
private:
	glm::vec3 computeNonNormalizedRayDirPinhole(glm::ivec2 pixel, glm::ivec2 frameDim)
	{
		// Compute sample position in screen space in [0,1] with origin at the top-left corner.
		// The camera jitter offsets the sample by +-0.5 pixels from the pixel center.
		glm::vec2 p = (glm::vec2(pixel) + glm::vec2(0.5f, 0.5f)) / glm::vec2(frameDim); // [0, 1]
		glm::vec2 ndc = glm::vec2(2, -2) * p + glm::vec2(-1, 1); // [-1, 1]

		// Compute the non-normalized ray direction assuming a pinhole camera.
		return ndc.x * m_view[0] + ndc.y * m_view[1] + m_view[2];
	}
	
	glm::mat4 m_view;
	glm::vec3 m_direction;
	float m_speed = 0.01f;
	glm::vec3 m_position;
	
	bool m_leftDown = false;
	bool m_rightDown = false;
	bool m_upDown = false;
	bool m_downDown = false;
};
