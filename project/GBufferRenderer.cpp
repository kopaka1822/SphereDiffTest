#include "GBufferRenderer.h"

GBufferRenderer::GBufferRenderer(Window& window)
	:
m_wnd(window),
m_cam(glm::vec3(0.0f))
{
	m_wnd.setKeyDownCallback([this](Window::Key key)
	{
		m_cam.onKey(key, true);

		switch (key)
		{
		case Window::Key::KP_ADD:
		case Window::Key::RIGHT_BRACKET: // + for me...
			m_wnd.setColorScale(m_wnd.getColorScale() * 2.0f);
			break;
		case Window::Key::KP_SUBTRACT:
		case Window::Key::SLASH:
			m_wnd.setColorScale(m_wnd.getColorScale() * 0.5f);
			break;
		}
	});
	m_wnd.setKeyUpCallback([this](Window::Key key)
	{
		m_cam.onKey(key, false);
	});
	m_wnd.setMouseDownCallback([this](Window::Button btn, float x, float y)
	{
		if(btn == Window::LEFT)
		{
			m_mouseDown = true;
		}
	});
	m_wnd.setMouseUpCallback([this](Window::Button btn, float x, float y)
	{
		if(btn == Window::LEFT)
		{
			m_mouseDown = false;
		}
	});
	m_wnd.setMouseMoveCallback([this](float x, float y, float dx, float dy)
	{
		if (m_mouseDown) m_cam.drag(dx, dy);
	});
}

void GBufferRenderer::render()
{
	// check size
	if(size_t(m_pixels.getWidth()) != m_wnd.getWidth() || size_t(m_pixels.getHeight()) != m_wnd.getHeight())
	{
		m_pixels = Pixels<GBuffer>(int(m_wnd.getWidth()), int(m_wnd.getHeight()));
		m_cam.setAspect(float(m_wnd.getWidth()) / float(m_wnd.getHeight()));
		m_timer.start();
	}

	m_pixels.clear();
	m_cam.update(m_timer.lap());
	
	// traverse scene and return foremost hit point

	RayData ray;
	ray.origin = glm::vec3(0.0);
	auto curPixel = m_pixels.begin();
	for(int y = 0; y < m_pixels.getHeight(); ++y)
	{
		for(int x = 0; x < m_pixels.getWidth(); ++x)
		{
			// generate ray
			ray = m_cam.computeRayPinhole(x, y, m_pixels.getWidth(), m_pixels.getHeight());
			auto isect =  m_scene.TraceRay(ray);
			
			GBuffer res;
			res.ray = ray;
			if(isect.has_value())
			{
				res.hit = isect.value();
			}
			
			
			*curPixel = res;
			++curPixel;
		}
	}

	assert(curPixel == m_pixels.end());
}
