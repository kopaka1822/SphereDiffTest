#include "App.h"
#include <sstream>

App::App()
	:
	m_wnd(160, 90, "RayDiff Tool"),
	m_attrib(Attribute::Depth),
	m_mode(DisplayMode::Diff)
{
	addFunction("close", script::Util::makeFunction(this, &App::close, "App::close()"));

	m_gbuffer.emplace(m_wnd);
}

App& App::get()
{
	// script objects need to be script ptr
	static script::ScriptPtr<App> app(new App);
	return *app;
}

bool App::run()
{
	m_wnd.handleEvents();

	m_gbuffer->render();
	m_attribRenderer.update(m_gbuffer->getPixels(), m_attrib);

	const auto& attribPixels = m_attribRenderer.getPixels();

	if(m_mode == DisplayMode::Reference)
	{
		m_wnd.swapBuffer(attribPixels);

		auto curColor = attribPixels.get(int(m_wnd.getMouseX()), int(m_wnd.getMouseY()));
		updateTitle(curColor);
	}
	else if(m_mode == DisplayMode::Diff)
	{
		m_pixels.resize(attribPixels.getWidth(), attribPixels.getHeight());
		
		Scissor s;
		s.xstart = 0; s.ystart = 0;
		s.yend = m_pixels.getHeight();
		s.xend = m_wnd.getMouseX() + 1;
		m_refDiff.update(m_gbuffer->getPixels(), attribPixels, m_pixels, s);
		auto leftColor = m_pixels.get(int(m_wnd.getMouseX()), int(m_wnd.getMouseY()));
		
		s.xstart = m_wnd.getMouseX();
		s.xend = m_wnd.getWidth();
		m_rayDiff.update(m_gbuffer->getPixels(), attribPixels, m_pixels, s, m_attrib);
		
		m_wnd.swapBuffer(m_pixels);

		auto rightColor = m_pixels.get(int(m_wnd.getMouseX()), int(m_wnd.getMouseY()));
		updateTitle(leftColor, rightColor);
	}

	return m_wnd.isOpen();
}

void App::close()
{
	m_wnd.close();
}

void App::updateTitle(glm::vec3 color)
{
	std::stringstream ss;
	ss << m_wnd.getMouseX() << "," << m_wnd.getMouseY() << ": " << color.r << " " << color.g << " " << color.b;
	m_wnd.setTitle(ss.str());
}

void App::updateTitle(glm::vec3 left, glm::vec3 right)
{
	std::stringstream ss;
	ss << m_wnd.getMouseX() << "," << m_wnd.getMouseY() << ": ";
	ss << left.r << " " << left.g << " " << left.b << " | ";
	ss << right.r << " " << right.g << " " << right.b << " Diff: ";
	auto diff = abs(left - right);
	ss << diff.r << " " << diff.g << " " << diff.b;
	
	m_wnd.setTitle(ss.str());
}
