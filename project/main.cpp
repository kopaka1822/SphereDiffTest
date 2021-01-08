#include <exception>
#include <iostream>
#include "App.h"
#include "../framework/glmmath.h"
#include "../dependencies/AsyncInput.h"
#include <script/statics/IOObject.h>

static AsyncInput input;

int main(int argc, const char** argv) try
{
	// redirect all console output to input instance
	std::cout.set_rdbuf(&input);
	std::cerr.set_rdbuf(&input);

	// init script engine + autocomplete
	script::ScriptEngine script;
	script.setStaticObject("App", App::get().shared_from_this());
	input.setAutoCompleteProvider([&script](const std::string& command)
	{
		return script.getAutocomplete(command);
	});

	// run command line scripts
	try
	{
		auto io = std::dynamic_pointer_cast<script::IOObject>(script.getStaticObject("IO"));
		io->setExecuteDebugOutput(true);
		for (int i = 1; i < argc; ++i)
			io->executeFile(argv[i]);
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERR: " << e.what() << '\n';
	}

	// main loop
	while (App::get().run())
	{
		std::string command = input.get();
		if (command.empty()) continue;
		try
		{
			std::cout << ">> " << script.execute(command)->toString() << '\n';
		}
		catch (const std::exception& e) 
		{
			std::cerr << "ERR: " << e.what() << '\n';
		}
	}
	return 0;
}
catch (const std::exception& e)
{
	std::cerr << "ERR: " << e.what();
	return 1;
}