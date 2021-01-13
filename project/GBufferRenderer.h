#pragma once
#include <variant>
#include "Camera.h"
#include "../framework/Timer.h"
#include "../framework/Window.h"
#include "Ray.h"
#include "Scene.h"
#include "Sphere.h"

struct MissData
{
	bool operator==(const MissData& right) const { return true; }
};

struct GBuffer
{
	std::variant<MissData, Sphere> hit;
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
	Scene m_scene;
	bool m_mouseDown = false;
};