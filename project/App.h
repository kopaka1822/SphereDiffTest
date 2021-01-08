#pragma once
#include "../framework/Window.h"
#include <script/ScriptEngine.h>

class App final : public script::ScriptObject
{
	App();
public:

	static App& get();

	// returns true as long as the application should be executed
	bool run();
	void close();
private:
	Window m_wnd;

};