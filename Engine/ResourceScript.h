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

	bool Update(float dt);

	void DeleteToMemory();
	bool LoadToMemory();
	Resource::State IsCompiled();

private:
	// 
	std::string path_assets;
	std::string path_dll;
	CSharpScript* csharp = nullptr;
};


#endif