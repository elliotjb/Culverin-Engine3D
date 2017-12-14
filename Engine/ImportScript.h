#ifndef _IMPORTSCRIPT_
#define _IMPORTSCRIPT_

// Info-> http://docs.go-mono.com/?link=root:/embed

#include "Globals.h"
#include <string>
#include <mono/jit/jit.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

class CSharpScript;

class ImportScript
{
public:
	ImportScript();
	~ImportScript();

	bool InitScriptingSystem();

	bool Import(const char* file, uint uuid = 0);
	int CompileScript(const char * file, std::string & libraryScript);
	CSharpScript* LoadScript_CSharp(std::string);
	CSharpScript* CreateCSharp(MonoImage* image);
	MonoClass* GetMonoClassFromImage(MonoImage * image, std::string& name_space, std::string& classname);

	MonoDomain* GetDomain() const;
	MonoImage* GetCulverinImage() const;
	std::string GetMonoPath() const;

private:
	void LinkFunctions();

	//CONSOLE FUNCTIONS ---------
	static void ConsoleLog(MonoString* string);

private:
	std::string mono_path;
	MonoDomain* domain = nullptr;
	MonoImage* culverin_mono_image = nullptr;
};

#endif