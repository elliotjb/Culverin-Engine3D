#include "ModuleScripting.h"
#include "Scripting/Scripting.h"
#include "Application.h"
#include "ModuleFS.h"

#pragma comment(lib, "Scripting/Scripting.lib")

ModuleScripting::ModuleScripting(bool start_enabled): Module(start_enabled)
{
	Awake_enabled = true;

	name = "Scripting";
}

ModuleScripting::~ModuleScripting()
{
}

bool ModuleScripting::Init(JSON_Object* node)
{
	perf_timer.Start();

	//Init the Mono Domain to work with scripts -----
	if (ScriptingSystem::InitSystem())
	{
		LOG("Mono Domain Init SUCCESS.");
	}
	else
	{
		LOG("Mono Domain Init FAIL.");
	}

	// COMPILATION TEST
	std::string libraryscript = App->fs->GetAssetsDirectory() + "/" + DIRECTORY_LIBRARY_SCRIPTS;
	const char*  file = ScriptingSystem::CompileFile("C:/Users/jordior4/Documents/GitHub/3D-Engine/Engine/Game/Assets/Example.cs", libraryscript.c_str());

	// EXECUTION A METHOD TEST
	std::string hello = ScriptingSystem::HelloWorld("C:/Users/jordior4/Documents/GitHub/3D-Engine/Engine/Game/Library/Scripts/Example.dll");
	LOG("%s", hello.c_str());

	Awake_t = perf_timer.ReadMs();
	return true;
}

update_status ModuleScripting::Update(float dt)
{
	// This will compile .cs files in runtime

	return UPDATE_CONTINUE;
}

bool ModuleScripting::CleanUp()
{
	//Shut down Mono ----------------
	ScriptingSystem::CloseSystem();

	return true;
}
