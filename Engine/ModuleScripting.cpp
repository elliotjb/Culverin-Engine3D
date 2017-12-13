#include "ModuleScripting.h"
#include "Scripting.h"
#include "Application.h"
#include "ModuleFS.h"


ModuleScripting::ModuleScripting(bool start_enabled): Module(start_enabled)
{
	Awake_enabled = true;
	Start_enabled = true;

	name = "Scripting";
}

ModuleScripting::~ModuleScripting()
{
}

bool ModuleScripting::Init(JSON_Object* node)
{
	perf_timer.Start();

	////Init the Mono Domain to work with scripts -----
	//if (ScriptingSystem::InitSystem())
	//{
	//	LOG("Mono Domain Init SUCCESS.");
	//}
	//else
	//{
	//	LOG("Mono Domain Init FAIL.");
	//}

	//// COMPILATION TEST
	//std::string libraryscript = App->fs->GetAssetsDirectory() + "/" + DIRECTORY_LIBRARY_SCRIPTS;
	//const char*  file = ScriptingSystem::CompileFile("C:/Users/Administrador/Documents/GitHub/3D-Engine/Engine/Game/Assets/Testelliot.cs", libraryscript.c_str());

	Awake_t = perf_timer.ReadMs();
	return true;
}

bool ModuleScripting::Start()
{
	perf_timer.Start();

	//const char* ello = ScriptingSystem::Hello("C:/Users/Administrador/Documents/GitHub/3D-Engine/Engine/Game/Library/Scripts/Testelliot.dll");

	// EXECUTION A METHOD TEST
	//const char* hello = ScriptingSystem::HelloWorld("C:/Users/Administrador/Documents/GitHub/3D-Engine/Engine/Game/Library/Scripts/Test.dll");
	int i = 0;
	//LOG(hello);
	//LOG("Hola");

	Start_t = perf_timer.ReadMs();
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
	//ScriptingSystem::CloseSystem();

	return true;
}
