#ifndef _IMPORTSCRIPT_
#define _IMPORTSCRIPT_

// Info-> http://docs.go-mono.com/?link=root:/embed

#include "Globals.h"
#include <string>


#include <mono/metadata/metadata.h>
#include <mono/metadata/object.h>

class CSharpScript;

class ImportScript
{
public:
	ImportScript();
	~ImportScript();

	bool Import(const char* file, uint uuid = 0);
	int CompileScript(const char * file, std::string & libraryScript);
	CSharpScript* LoadScript_CSharp(std::string);
	CSharpScript* CreateCSharp(MonoImage* image);
	MonoClass* GetMonoClassFromImage(MonoImage * image, std::string& name_space, std::string& classname);
};

#endif