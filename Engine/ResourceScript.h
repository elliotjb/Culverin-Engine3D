#ifndef _RESOURCESCRIPT_
#define _RESOURCESCRIPT_

#include "Resource_.h"
#include "Globals.h"

#include <string>

class CSharpScript;
class Script_editor;

class ResourceScript : public Resource
{
public:
	ResourceScript(uint uid);
	virtual ~ResourceScript();

	void InitInfo(std::string path_dll_, std::string path_assets_);
	void SetCSharp(CSharpScript* csharp_);

	// Name Class = name Script
	void CreateNewScript(std::string nameScript);
	void SetScriptEditor(std::string nameScript);

	void FreeMono();

	bool Update(float dt);

	std::string GetPathAssets() const;
	void ShowEditor(bool & active);
	void DeleteToMemory();
	bool LoadToMemory();
	Resource::State IsCompiled();

private:
	// 
	std::string path_assets;
	std::string path_dll;
	CSharpScript* csharp = nullptr;
	Script_editor* editor = nullptr;
};


#endif