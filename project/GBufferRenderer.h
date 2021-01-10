#pragma once
#include <variant>


#include "Camera.h"
#include "../framework/Timer.h"
#include "../framework/Window.h"
#include "Ray.h"

struct MissData
{
	
};

struct SphereData
{
	glm::vec3 center;
	float radius;
};

struct GBuffer
{
	std::variant<MissData, SphereData> hit;
	RayData ray;
};

class GBufferRenderer
{
public:
	GBufferRenderer(Window& window);
	void render(); // refreshes everything
	const Pixels<GBuffer>& getPixels() const { return m_pixels; }
private:
	Pixels<GBuffer> m_pixels;
	Window& m_wnd;
	Camera m_cam;
	Timer m_timer;
};