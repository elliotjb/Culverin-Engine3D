#include "ModuleScripting.h"
#include "Scripting.h"
#include "Application.h"
#include "ModuleFS.h"

#include <mono/metadata/metadata.h>

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
	//This will compile .cs files in runtime
	return UPDATE_CONTINUE;
}

bool ModuleScripting::CleanUp()
{
	//Shut down Mono ----------------
	//ScriptingSystem::CloseSystem();

	return true;
}

void ModuleScripting::AddInternalCalls()
{
	////GAMEOBJECT FUNCTIONS ---------------
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::CreateGameObject",(const void*)CreateGameObject);
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::Destroy",(const void*)DestroyGameObject);
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::SetActive",(const void*)SetActive);
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::IsActive",(const void*)IsActive);
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::SetParent",(const void*)SetParent);
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::SetName",(const void*)SetName);
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::GetName",(const void*)GetName);
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::AddComponent",(const void*)AddComponent);
	//ScriptingSystem::AddInternalCall("CulverinEditor.GameObject::GetComponent",(const void*)GetComponent);

	////CONSOLE FUNCTIONS ------------------
	//ScriptingSystem::AddInternalCall("CulverinEditor.Console.Console::Log",(const void*)ConsoleLog);

	////INPUT FUNCTIONS -------------------
	//ScriptingSystem::AddInternalCall("CulverinEditor.Input::KeyDown",(const void*)KeyDown);
	//ScriptingSystem::AddInternalCall("CulverinEditor.Input::KeyUp",(const void*)KeyUp);
	//ScriptingSystem::AddInternalCall("CulverinEditor.Input::KeyRepeat",(const void*)KeyRepeat);
	//ScriptingSystem::AddInternalCall("CulverinEditor.Input::MouseButtonDown",(const void*)MouseButtonDown);
	//ScriptingSystem::AddInternalCall("CulverinEditor.Input::MouseButtonUp",(const void*)MouseButtonUp);
	//ScriptingSystem::AddInternalCall("CulverinEditor.Input::MouseButtonRepeat",(const void*)MouseButtonRepeat);



}
