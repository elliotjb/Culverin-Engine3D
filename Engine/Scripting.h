#ifndef SCRIPTING_MANAGER_H
#define SCRIPTING_MANAGER_H

#include <string>

namespace ScriptingSystem
{
	 bool InitSystem();
	 bool CloseSystem();


	 const char* CompileFile(const char* path, const char* dest_path);
	 const char* Hello(const char* path);
	 const char* HelloWorld(const char* path);

	// UTILITY FUNCTIONS ---------------------------------------------
	 void GetPath();
	 const char* GetMonoPath();
	 std::string GetName(const char* dll_path);
}

#endif;