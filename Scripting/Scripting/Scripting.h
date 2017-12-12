#ifndef SCRIPTING_MANAGER_H
#define SCRIPTING_MANAGER_H

#ifdef SCRIPTING_EXPORTS
#define SCRIPTING_MANAGER __declspec(dllexport)
#else
#define SCRIPTING_MANAGER __declspec(dllimport)
#endif

#include <string>

namespace ScriptingSystem
{
	SCRIPTING_MANAGER bool InitSystem();
	SCRIPTING_MANAGER bool CloseSystem();


	SCRIPTING_MANAGER const char* CompileFile(const char* path, const char* dest_path);
	SCRIPTING_MANAGER std::string HelloWorld(const char* path);

	// UTILITY FUNCTIONS ---------------------------------------------
	SCRIPTING_MANAGER void GetPath();
	SCRIPTING_MANAGER const char* GetMonoPath();
	SCRIPTING_MANAGER std::string GetName(const char* dll_path);
}

#endif;