#include "GBufferRenderer.h"

GBufferRenderer::GBufferRenderer(Window& window)
	:
m_wnd(window),
m_cam(glm::vec3(0.0f))
{
	m_wnd.setKeyDownCallback([this](Window::Key key)
	{
		m_cam.onKey(key, true);
	});
	m_wnd.setKeyUpCallback([this](Window::Key key)
	{
		m_cam.onKey(key, false);
	});
}

void GBufferRenderer::render()
{
	// check size
	if(size_t(m_pixels.getWidth()) != m_wnd.getWidth() || size_t(m_pixels.getHeight()) != m_wnd.getHeight())
	{
		m_pixels = Pixels<GBuffer>(int(m_wnd.getWidth()), int(m_wnd.getHeight()));
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

			GBuffer res;
			res.ray = ray;
			
			
			*curPixel = res;
			++curPixel;
		}
	}

	assert(curPixel == m_pixels.end());
}
