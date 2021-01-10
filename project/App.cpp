#include "App.h"

App::App()
	:
	m_wnd(800, 600, "RayDiff Tool")
{
	addFunction("close", script::Util::makeFunction(this, &App::close, "App::close()"));
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

	m_wnd.swapBuffer();

	return m_wnd.isOpen();
}

void App::close()
{
	m_wnd.close();
}
