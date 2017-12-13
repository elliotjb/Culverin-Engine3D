#ifndef _RESOURCESCRIPT_
#define _RESOURCESCRIPT_

#include "Resource_.h"
#include "Globals.h"

#include <string>

class CSharpScript;

class ResourceScript : public Resource
{
public:
	ResourceScript(uint uid);
	virtual ~ResourceScript();

	void InitInfo(std::string path_dll_, std::string path_assets_);
	void SetCSharp(CSharpScript* csharp_);

	void DeleteToMemory();
	bool LoadToMemory();
	Resource::State IsLoadedToMemory();

private:
	// 
	std::string path_assets;
	std::string path_dll;
	CSharpScript* csharp = nullptr;
};


#endif