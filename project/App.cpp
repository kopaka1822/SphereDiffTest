#include "App.h"

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
	
	m_wnd.swapBuffer(m_attribRenderer.getPixels());

	return m_wnd.isOpen();
}

void App::close()
{
	m_wnd.close();
}
