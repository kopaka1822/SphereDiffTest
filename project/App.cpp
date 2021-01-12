#include "App.h"
#include <sstream>

App::App()
	:
	m_wnd(720, 480, "RayDiff Tool")
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
	m_attribRenderer.update(m_gbuffer->getPixels());

	const auto& pixels = m_attribRenderer.getPixels();
	m_wnd.swapBuffer(pixels);
	
	auto curColor = pixels.get(int(m_wnd.getMouseX()), int(m_wnd.getMouseY()));
	updateTitle(curColor);

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
